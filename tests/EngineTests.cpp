#include "native/Engine.hpp"
#include "native/Registry.hpp"
#include "native/Types.hpp"

#include <cassert>

int main()
{
    native::Registry registry;
    const native::Entity first = registry.create();
    assert(registry.alive(first));
    auto& transform = registry.emplace<native::Transform>(first);
    transform.position.x = 2.0F;
    assert(registry.has<native::Transform>(first));
    assert(registry.get<native::Transform>(first)->position.x == 2.0F);
    assert(registry.destroy(first));
    assert(!registry.alive(first));
    const native::Entity recycled = registry.create();
    assert(recycled.index == first.index);
    assert(recycled.generation != first.generation);
    assert(registry.get<native::Transform>(recycled) == nullptr);

    native::Input input;
    input.set(native::Key::w, true);
    assert(input.down(native::Key::w));
    assert(input.pressed(native::Key::w));
    input.end_frame();
    assert(!input.pressed(native::Key::w));
    input.set(native::Key::w, false);
    assert(input.released(native::Key::w));

    native::SoftwareRenderer renderer;
    assert(renderer.initialize(8, 8));
    renderer.begin_frame({0, 0, 0, 255});
    renderer.draw_rect({-2, -2, 4, 4}, {255, 0, 0, 255});
    assert(renderer.pixels().size() == 64);
    assert(renderer.pixels()[0] == native::Color{255, 0, 0, 255});
    renderer.end_frame();
    assert(renderer.frames_rendered() == 1);
    assert(renderer.checksum() != 0);
    renderer.shutdown();

    native::HeadlessWindow window(2);
    native::SoftwareRenderer engine_renderer;
    native::Engine engine;
    assert(engine.initialize(window, engine_renderer, {"test", 8, 8}));
    engine.run({}, [](native::SoftwareRenderer& target) {
        target.draw_rect({1, 1, 2, 2}, {1, 2, 3, 255});
    });
    assert(engine_renderer.frames_rendered() == 1);
    engine.shutdown();
    assert(!engine.running());
    return 0;
}
