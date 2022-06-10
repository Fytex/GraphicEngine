from math import sqrt, pi, cos, sin, atan2, asin

def to_cartesian(r, alpha, beta):
    return (r * sin(alpha) * cos(beta), r * sin(beta), r * cos(beta) * cos(alpha))

def to_polar(x, y, z):
    r = sqrt(x**2 + y**2 + z**2)
    return (r, atan2(x, z), r and asin(y / r))

def create_orbit(point, num_points=None, center=(0,0,0)):
    points = []

    pos = to_polar(point[0] - center[0], point[1] - center[1], point[2] - center[2])
    
    if num_points is None:
        num_points = int((2 * pi * pos[0]) / 2)

    vec = (center[0] - point[0], center[1] - point[1], center[2] - point[2])
    radius = sqrt(vec[0]**2 + vec[1]**2 + vec[2]**2);


    
        


    #alpha_inc = 2 * pi / num_points
    #beta_inc_total = 4 * abs(pos[2])
    #print(beta_inc_total)

    #alpha_inc_total = 2 * pi - beta_inc_total / 2

    #beta_inc = beta_inc_total / num_points
    #alpha_inc = alpha_inc_total / num_points

    inc = to_polar(*point)[2]
    max_height = sin(inc) * radius

    for i in range(num_points):    

        phase = i / num_points
        x = cos(2 * pi * phase) * radius
        z = sin(2 * pi * phase) * radius

        y = sin(2 * pi * phase + pi/2) * max_height


        points.append((round(x, 3) + center[0], round(y, 3) + center[1], round(z, 3) + center[2]))
        
    return points

    
