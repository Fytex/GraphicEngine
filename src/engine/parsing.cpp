#include "pugixml.hpp"
#include "../utils/point.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "group.hpp"
#include "color.hpp"
#include "light.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

void parseModel(Model &model, pugi::xml_node toolModel) {
    
    std::string file(toolModel.attribute("file").as_string());
    std::string texture(toolModel.child("texture").attribute("file").as_string());

    pugi::xml_node color_node(toolModel.child("color"));

    pugi::xml_node diffuse_node(color_node.child("diffuse"));
    pugi::xml_node ambient_node(color_node.child("ambient"));
    pugi::xml_node specular_node(color_node.child("specular"));
    pugi::xml_node emissive_node(color_node.child("emissive"));
    pugi::xml_node shininess_node(color_node.child("shininess"));

    Color color_diffuse(
        diffuse_node.attribute("R").as_int(200),
        diffuse_node.attribute("G").as_int(200),
        diffuse_node.attribute("B").as_int(200)
    );

    Color color_ambient(
        ambient_node.attribute("R").as_int(50),
        ambient_node.attribute("G").as_int(50),
        ambient_node.attribute("B").as_int(50)
    );

    Color color_specular(
        specular_node.attribute("R").as_int(0),
        specular_node.attribute("G").as_int(0),
        specular_node.attribute("B").as_int(0)
    );

    Color color_emissive(
        emissive_node.attribute("R").as_int(0),
        emissive_node.attribute("G").as_int(0),
        emissive_node.attribute("B").as_int(0)
    );

    int color_shininess = shininess_node.attribute("value").as_int(0);

    model.init(file, texture, color_diffuse, color_ambient, color_specular, color_emissive, color_shininess);
}

void parseGroup(Group &parent_group, pugi::xml_node toolParentGroup) {

    for (pugi::xml_node toolModel : toolParentGroup.child("models").children("model")) {
        Model model;

        parseModel(model, toolModel);
        parent_group.add_model(model);
    }

    for (pugi::xml_node tool : toolParentGroup.child("transform").children()) {
        enum Transformation transformation = transformationStringToEnum(tool.name());
        pugi::xml_attribute  time_attr;
        

        switch (transformation)
        {
        case Transformation::TRANSLATION:
            time_attr = tool.attribute("time");

            if (time_attr.empty())
                parent_group.add_translation(Translation(tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float()));
            else {
                std::vector<Point> points;
                for (pugi::xml_node toolPoint : tool.children("point")) {
                    points.emplace_back(Point(toolPoint.attribute("x").as_float(), toolPoint.attribute("y").as_float(), toolPoint.attribute("z").as_float()));
                }

                parent_group.add_translation(Translation(time_attr.as_float(), tool.attribute("align").as_bool(), points));
            }
            break;
        case Transformation::ROTATION:
            parent_group.add_rotation(Rotation(tool.attribute("time").as_float(), tool.attribute("angle").as_float(), tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float()));
            break;
        case Transformation::SCALE:
            parent_group.add_scale(Scale(tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float()));
            break;
        }
    }



    for (pugi::xml_node toolGroup : toolParentGroup.children("group")) {
        Group group;

        parseGroup(group, toolGroup);
        parent_group.add_group(group);
    }
}

void parseLights(std::vector<Light>& lights, pugi::xml_node toolLights) {

    for (pugi::xml_node toolLight : toolLights.children("light")) {
        Light light(
            lightTypeStringToEnum(toolLight.attribute("type").as_string()),
            Point(
                toolLight.attribute("posX").as_float(),
                toolLight.attribute("posY").as_float(),
                toolLight.attribute("posZ").as_float()
            ),
            Vector(
                toolLight.attribute("dirX").as_float(),
                toolLight.attribute("dirY").as_float(),
                toolLight.attribute("dirZ").as_float()
            ),
            toolLight.attribute("cutoff").as_float()
        );

        lights.push_back(light);
    }
}

void parseCamera(Camera& camera, pugi::xml_node toolCamera) {
    pugi::xml_node tool;

    tool = toolCamera.child("position");
    Point eye(tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float());

    tool = toolCamera.child("lookAt");
    Point center(tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float());

    tool = toolCamera.child("up");
    Vector up(tool.attribute("x").as_float(), tool.attribute("y").as_float(), tool.attribute("z").as_float());

    tool = toolCamera.child("projection");
    double fov = tool.attribute("fov").as_double();
    double near = tool.attribute("near").as_double();
    double far = tool.attribute("far").as_double();


    camera.init(eye, center, up, fov, near, far);
}

bool parse(char * filename, Camera &camera, std::vector<Light>& lights, Group& group) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);

    if (!result) {
        return false;
    }

    pugi::xml_node toolWorld = doc.child("world");

    if (!toolWorld)
        throw std::invalid_argument("World not found in .xml");


    pugi::xml_node toolCamera = toolWorld.child("camera");

    if (!toolCamera)
        throw std::invalid_argument("Camera not found in .xml's World");

    parseCamera(camera, toolCamera);


    pugi::xml_node toolLights = toolWorld.child("lights");

    parseLights(lights, toolLights);


    pugi::xml_node toolGroup = toolWorld.child("group");

    if (!toolGroup)
        throw std::invalid_argument("Group not found in .xml's World");

    parseGroup(group, toolGroup);

    return true;
}