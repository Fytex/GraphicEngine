#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <tuple>

#include "parsing.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "group.hpp"
#include "light.hpp"
#include "catmull_rom.hpp"
#include "../utils/vector.hpp"
#include "../utils/point.hpp"
#include "../utils/point2D.hpp"
#include "../utils/spherical_coord.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

static Camera camera;
static Group main_group;
static std::vector<Light> lights;
static std::unordered_map<std::string, std::tuple<GLuint, GLuint, GLuint, long>> models_buffers;
static std::unordered_map<std::string, unsigned int> textures_buffers;

static GLenum current_mode = GL_FILL;

static bool show_axis = true;
static bool show_aim = true;

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio 
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(camera.fov, ratio, camera.near, camera.far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderGroup(Group& group) {
    float time;
    int time_milis;

    for (auto const& transformation : group.ordered_transformations) {
        switch (transformation)
        {
        case Transformation::TRANSLATION:
            time = group.translation.time;

            if (time > 0) {
                time_milis = time * 1000; // miliseconds

                if (show_axis) {
                    glDisable(GL_LIGHTING);
                    glBegin(GL_LINE_LOOP);
                    for (int i = 0; i < 100; ++i) {
                        auto [pos, deriv] = getGlobalCatmullRomPoint(group.translation.points, ((float)i) / 100.0f);
                        glVertex3f(pos.x, pos.y, pos.z);
                    }
                    glEnd();
                    glEnable(GL_LIGHTING);
                }


                auto [pos, deriv] = getGlobalCatmullRomPoint(group.translation.points, ((float) (glutGet(GLUT_ELAPSED_TIME) % time_milis)) / time_milis);

                glTranslatef(pos.x, pos.y, pos.z);

                if (group.translation.align) {
                    float m[16];
                    static Vector vec_y(0, 1, 0);
                    
                    deriv.normalize();

                    Vector vec_z = deriv.cross(vec_y);
                    vec_z.normalize();

                    vec_y = vec_z.cross(deriv);
                    vec_y.normalize();

                    buildRotMatrix(deriv, vec_y, vec_z, m);
                    glMultMatrixf(m);
                }
            }
            else
                glTranslatef(group.translation.x, group.translation.y, group.translation.z);
            break;
        case Transformation::ROTATION:
            time = group.rotation.time;

            if (time > 0) {
                time_milis = time * 1000; // miliseconds
                glRotatef((((float)(glutGet(GLUT_ELAPSED_TIME) % time_milis)) / time_milis) * 360 - 180, group.rotation.axis_x, group.rotation.axis_y, group.rotation.axis_z);
            }
            else
                glRotatef(group.rotation.alpha, group.rotation.axis_x, group.rotation.axis_y, group.rotation.axis_z);
            break;
        case Transformation::SCALE:
            glScalef(group.scale.x, group.scale.y, group.scale.z);
            break;
        }
    }


    for (Model& model : group.models) {
        auto [vbuffer, nbuffer, tbuffer, total_vertices] = models_buffers.at(model.file);


        glMaterialfv(GL_FRONT, GL_DIFFUSE, model.color_diffuse.getVectorf().data());
        glMaterialfv(GL_FRONT, GL_AMBIENT, model.color_ambient.getVectorf().data());
        glMaterialfv(GL_FRONT, GL_SPECULAR, model.color_specular.getVectorf().data());
        glMaterialfv(GL_FRONT, GL_EMISSION, model.color_emissive.getVectorf().data());
        glMateriali(GL_FRONT, GL_SHININESS, model.color_shininess);

        glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, nbuffer);
        glNormalPointer(GL_FLOAT, 0, 0);

        if (!model.texture.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);

            glBindTexture(GL_TEXTURE_2D, textures_buffers.at(model.texture));
            glDrawArrays(GL_TRIANGLES, 0, total_vertices);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else
            glDrawArrays(GL_TRIANGLES, 0, total_vertices);

    }


    for (auto& sub_group : group.groups) {
        glPushMatrix();
        renderGroup(sub_group);
        glPopMatrix();
    }

}

void updateCamera(void) {
    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,
        camera.center.x, camera.center.y, camera.center.z,
        camera.up.vx, camera.up.vy, camera.up.vz);
}


void renderLights(void) {
    int i = 0;
    GLuint light_idx;

    for (Light& light : lights) {
        light_idx = GL_LIGHT0 + i;
        glLightfv(light_idx, GL_POSITION, light.getSourceVectorf().data());

        
        if (light.type == SPOTLIGHT) {
            glLightfv(light_idx, GL_SPOT_DIRECTION, light.getSpotLightDirVectorf().data());
            glLightf(light_idx, GL_SPOT_CUTOFF, light.cut_off);
        }

        ++i;
    }
}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    updateCamera();

    if (show_aim || show_axis) {
        glDisable(GL_LIGHTING);

        if (show_axis) {
            glBegin(GL_LINES);
            // X axis in red
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-100.0f, 0.0f, 0.0f);
            glVertex3f(100.0f, 0.0f, 0.0f);
            // Y Axis in Green
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, -100.0f, 0.0f);
            glVertex3f(0.0f, 100.0f, 0.0f);
            // Z Axis in Blue
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, -100.0f);
            glVertex3f(0.0f, 0.0f, 100.0f);
            glEnd();
        }
        if (show_aim) {
            glColor3f(0.8f, 0.2f, 1.0f);
            glPushMatrix();
            Vector vec(Point(), camera.center);
            glTranslatef(vec.vx, vec.vy, vec.vz);
            glutSolidSphere(.05, 10, 10);
            glPopMatrix();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
    }


    renderLights();
    renderGroup(main_group);

    // End of frame
    glutSwapBuffers();
}

void loadModels(const Group& group) {
    long total_vertices;
    float x, y, z, nx, ny, nz, tx, ty;

    for (const auto& model : group.models) {
        bool has_texture = !model.texture.empty();

        if (!models_buffers.contains(model.file)) {
            std::ifstream stream(model.file);
            std::ifstream tex_stream;

            if (!stream)
                throw std::invalid_argument(model.file + " can't be opened");

            if (has_texture) {
                tex_stream.open(model.file + ".tex");

                if (!tex_stream)
                    throw std::invalid_argument(model.file + ".tex can't be opened");
            }


            stream >> total_vertices;

            GLuint buffers[3];

            glGenBuffers(has_texture ? 3 : 2, buffers);

            models_buffers.emplace(model.file, std::make_tuple(buffers[0], buffers[1], buffers[2], total_vertices));

            std::vector<Point> vertices;
            std::vector<Vector> normals;
            

            vertices.reserve(total_vertices);
            normals.reserve(total_vertices);

            while (stream >> x >> y >> z >> nx >> ny >> nz) {
                vertices.emplace_back(x, y, z);
                normals.emplace_back(nx, ny, nz);
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, 3 * total_vertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, 3 * total_vertices * sizeof(float), normals.data(), GL_STATIC_DRAW);

            

            if (has_texture) {
                std::vector<Point2D> tex_coords;

                tex_coords.reserve(total_vertices);

                while (tex_stream >> tx >> ty)
                    tex_coords.emplace_back(tx, ty);

                glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
                glBufferData(GL_ARRAY_BUFFER, 2 * total_vertices * sizeof(float), tex_coords.data(), GL_STATIC_DRAW);
            }
        }


        if (has_texture && !textures_buffers.contains(model.texture)) {
            unsigned int t, tw, th;
            unsigned char * tex_data;

            ilGenImages(1, &t);
            ilBindImage(t);
            ilLoadImage((ILstring) model.texture.c_str());
            tw = ilGetInteger(IL_IMAGE_WIDTH);
            th = ilGetInteger(IL_IMAGE_HEIGHT);
            ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            tex_data = ilGetData();

            GLuint texID;
            glGenTextures(1, &texID);

            textures_buffers.emplace(model.texture, texID);

            glBindTexture(GL_TEXTURE_2D, texID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

            ilDeleteImages(1, &t);
        }
    }

    for (const auto& sub_group : group.groups)
        loadModels(sub_group);
}

bool initContext(char * filename) {
    
    if (!parse(filename, camera, lights, main_group))
        return false;

    loadModels(main_group);

    return true;
}


// write function to process keyboard events

void clickResumeWindow(int button, int state, int x, int y);

void keyboardKeysFunc(unsigned char key, int x, int y) {

    float step_var = 0.2f;
    Vector vec(camera.eye, camera.center);
    vec.normalize();

    switch (key) {
    case 27:
        glutMotionFunc(NULL);
        glutPassiveMotionFunc(NULL);
        glutSpecialFunc(NULL);
        glutKeyboardFunc(NULL);
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        glutMouseFunc(clickResumeWindow);
        break;

    case 'z':
        switch (current_mode) {
        case GL_FILL: current_mode = GL_LINE; break;
        case GL_LINE: current_mode = GL_POINT; break;
        default: current_mode = GL_FILL;
        }

        glPolygonMode(GL_FRONT, current_mode);
        break;

    case 'x':
        show_axis = !show_axis;
        break;

    case 'c':
        show_aim = !show_aim;
        break;

    case 'w':
        vec.multiply(step_var);
        camera.eye.addVector(vec);
        camera.center.addVector(vec);
        break;

    case 's':
        vec.multiply(-step_var);
        camera.eye.addVector(vec);
        camera.center.addVector(vec);
        break;

    case 'd':
        vec.multiply(step_var);
        vec = vec.cross(camera.up);
        camera.eye.addVector(vec);
        camera.center.addVector(vec);
        break;

    case 'a':
        vec.multiply(-step_var);
        vec = vec.cross(camera.up);
        camera.eye.addVector(vec);
        camera.center.addVector(vec);
        break;
    }


    glutPostRedisplay();
}

void specialKeysFunc(int key_code, int x, int y) {
    Vector recenter_vec = Vector(camera.center, Point());

    Point camera_eye(camera.eye);
    camera_eye.addVector(recenter_vec);

    SphericalCoord eye_camera_sc = SphericalCoord(camera_eye);
    
    float alpha_var = M_PI_4 / 20.0f;
    float beta_var = M_PI_4 / 20.0f;
    float radius_var = 0.5f;


    switch (key_code) {
    case GLUT_KEY_UP:
        if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {

            if (eye_camera_sc.radius > radius_var)
                eye_camera_sc.radius -= radius_var;

        }
        else {

                // Beta angle changes perspective if it hits -PI/2 or PI/2. This code avoids that
            if (eye_camera_sc.beta < M_PI_2 - beta_var)
                eye_camera_sc.beta += beta_var;

        }
        break;
    case GLUT_KEY_DOWN:
        if (glutGetModifiers() & GLUT_ACTIVE_CTRL)
            eye_camera_sc.radius += radius_var;
        else {
            // Beta angle changes perspective if it hits -PI/2 or PI/2. This code avoids that
            if (eye_camera_sc.beta > -M_PI_2 + beta_var)
                eye_camera_sc.beta -= beta_var;
        }
        break;
    case GLUT_KEY_RIGHT: eye_camera_sc.alpha += alpha_var; break;
    case GLUT_KEY_LEFT: eye_camera_sc.alpha -= alpha_var; break;
    default: return;
    }

    camera.eye = Point(eye_camera_sc);

    recenter_vec.multiply(-1);
    camera.eye.addVector(recenter_vec);
    
    updateCamera();

    glutPostRedisplay();
}

void mouseFunc(int x, int y) {
    int size_width = glutGet(GLUT_WINDOW_WIDTH);
    int size_height = glutGet(GLUT_WINDOW_HEIGHT);

    int center_width = size_width / 2;
    int center_height = size_height / 2;

    if (x == center_width && y == center_height)
        return;

    float alpha = M_PI_2 * (((float)(x - center_width)) / (float)size_width);
    float beta = M_PI_2 * (((float)(y - center_height)) / (float)size_height);



    Point camera_fake_center = camera.center;
    Vector vec_recenter(camera.eye, Point());
    camera_fake_center.addVector(vec_recenter);


    SphericalCoord center_sc(camera_fake_center);

    center_sc.alpha -= alpha;
    center_sc.beta -= beta;

    if (center_sc.beta >= M_PI_2 || center_sc.beta <= -M_PI_2)
        center_sc.beta += beta;

    camera.center = Point(center_sc);
    vec_recenter.multiply(-1);
    camera.center.addVector(vec_recenter);

    glutWarpPointer(center_width, center_height);
    glutPostRedisplay();
}


void updateInputCallbacks(void) {
    // put here the registration of the keyboard callbacks
    glutSpecialFunc(specialKeysFunc);
    glutKeyboardFunc(keyboardKeysFunc);

    // put here the registration of the mouse callbacks
    glutMotionFunc(mouseFunc);
    glutPassiveMotionFunc(mouseFunc);

    // Remove mouse cursor
    glutSetCursor(GLUT_CURSOR_NONE);
}

void clickResumeWindow(int button, int state, int x, int y) {
    // Save the left button state
    if (button == GLUT_LEFT_BUTTON) {
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
        updateInputCallbacks();
        glutMouseFunc(NULL);
    }
}






int main(int argc, char ** argv) {

    if (argc != 2) {
        std::cout << "Error: Engine requires .xml path" << std::endl;
        return -1;
    }
    

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("GraphicEngine");

    // init GLEW
#ifndef __APPLE__
    glewInit();
#endif

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);


    try {
        if (!initContext(argv[1])) {
            std::cout << "Error: Provided .xml file is corrupted or not found" << std::endl;
            return -1;
        }
    } catch (std::invalid_argument const& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return -1;
    }


    

    // Required callback registry 
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glPolygonMode(GL_FRONT, GL_FILL);


    updateInputCallbacks();
    
    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    //GLfloat dark[4] = { 0.3,0.3,0.3,1.0 };
    GLfloat white[4] = { 1.0,1.0,1.0,1.0 };
    //GLfloat black[4] = { 0.0,0.0,0.0,1.0 };

    // Lightning
    glEnable(GL_LIGHTING);
    glEnable(GL_RESCALE_NORMAL);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);

    for (size_t i = 0; i < lights.size(); ++i) {
        GLenum light = GL_LIGHT0 + i;
        glEnable(light);
        //glLightfv(light, GL_AMBIENT, dark);
        glLightfv(light, GL_DIFFUSE, white);
        glLightfv(light, GL_SPECULAR, white);
    }

    // Textures
    glEnable(GL_TEXTURE_2D);


    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
