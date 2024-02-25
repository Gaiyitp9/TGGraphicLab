#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Camera.h"

int main()
{
    Scene scene;
    scene.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    scene.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera;
    camera.Render("image.jpg", scene);

    return 0;
}
