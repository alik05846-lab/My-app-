#include "native/Engine.hpp"
#include "native/Types.hpp"

#include <iostream>

int main()
{
    native::HeadlessWindow window(4);
    native::SoftwareRenderer renderer;
    native::Engine engine;
    const native::WindowConfig config{"Native Engine Demo", 320, 180};

    if (!engine.initialize(window, renderer, config))
    {
        std::cerr << "Engine initialization failed\n";
        return 1;
    }

    engine.run(
        [](float) {},
        [](native::SoftwareRenderer& target)
        {
            target.draw_rect({40, 30, 120, 80}, {40, 160, 240, 255});
            target.draw_rect({80, 60, 120, 80}, {240, 80, 40, 255});
        });

    std::cout << "Rendered " << renderer.frames_rendered()
              << " frames; framebuffer checksum: " << renderer.checksum() << '\n';
    engine.shutdown();
    return 0;
}
