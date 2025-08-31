module;

#include <functional>
#include <utility>

export module Gfx.Reflection.TextureCoordinates;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;
export import Gfx.TextureCoordinates;

template<>
void Core::reflect<Gfx::TextureCoordinates>(ReflectionContext& context) {
    using namespace Gfx;
    auto classReflection = ClassReflectionBuilder<TextureCoordinates>("TextureCoordinates")
        .property("x0", &TextureCoordinates::x0)
        .property("y0", &TextureCoordinates::y0)
        .property("x1", &TextureCoordinates::x1)
        .property("y1", &TextureCoordinates::y1)
        .build();

    context.addClass(TypeId::get<TextureCoordinates>(), std::move(classReflection));
} // Gfx
