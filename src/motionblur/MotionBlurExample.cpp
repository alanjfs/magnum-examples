/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include "SceneGraph/Scene.h"
#include "MeshTools/CompressIndices.h"
#include "MeshTools/Interleave.h"
#include <Platform/GlutApplication.h>
#include "Primitives/Icosphere.h"
#include "Shaders/PhongShader.h"

#include "MotionBlurCamera.h"
#include "Icosphere.h"

using namespace Corrade;
using namespace Magnum::Shaders;

namespace Magnum { namespace Examples {

class MotionBlurExample: public Platform::GlutApplication {
    public:
        MotionBlurExample(int& argc, char** argv): GlutApplication(argc, argv, "Motion blur example") {
            (cameraObject = new Object3D(&scene))
                ->translate(Vector3::zAxis(3.0f));
            (camera = new MotionBlurCamera(cameraObject))
                ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
                ->setPerspective(35.0_degf, 1.0f, 0.001f, 100);
            Renderer::setClearColor({0.1f, 0.1f, 0.1f});
            Renderer::setFeature(Renderer::Feature::DepthTest, true);
            Renderer::setFeature(Renderer::Feature::FaceCulling, true);

            Primitives::Icosphere<3> data;
            MeshTools::compressIndices(&mesh, &indexBuffer, Buffer::Usage::StaticDraw, *data.indices());
            MeshTools::interleave(&mesh, &buffer, Buffer::Usage::StaticDraw, *data.positions(0), *data.normals(0));
            mesh.addInterleavedVertexBuffer(&buffer, 0, PhongShader::Position(), PhongShader::Normal());

            /* Add spheres to the scene */
            new Icosphere(&mesh, &shader, {1.0f, 1.0f, 0.0f}, &scene, &drawables);

            spheres[0] = new Object3D(&scene);
            (new Icosphere(&mesh, &shader, {1.0f, 0.0f, 0.0f}, spheres[0], &drawables))
                ->translate(Vector3::yAxis(0.25f));
            (new Icosphere(&mesh, &shader, {1.0f, 0.0f, 0.0f}, spheres[0], &drawables))
                ->translate(Vector3::yAxis(0.25f))
                ->rotateZ(120.0_degf);
            (new Icosphere(&mesh, &shader, {1.0f, 0.0f, 0.0f}, spheres[0], &drawables))
                ->translate(Vector3::yAxis(0.25f))
                ->rotateZ(240.0_degf);

            spheres[1] = new Object3D(&scene);
            (new Icosphere(&mesh, &shader, {0.0f, 1.0f, 0.0f}, spheres[1], &drawables))
                ->translate(Vector3::yAxis(0.50f));
            (new Icosphere(&mesh, &shader, {0.0f, 1.0f, 0.0f}, spheres[1], &drawables))
                ->translate(Vector3::yAxis(0.50f))
                ->rotateZ(120.0_degf);
            (new Icosphere(&mesh, &shader, {0.0f, 1.0f, 0.0f}, spheres[1], &drawables))
                ->translate(Vector3::yAxis(0.50f))
                ->rotateZ(240.0_degf);

            spheres[2] = new Object3D(&scene);
            (new Icosphere(&mesh, &shader, {0.0f, 0.0f, 1.0f}, spheres[2], &drawables))
                ->translate(Vector3::yAxis(0.75f));
            (new Icosphere(&mesh, &shader, {0.0f, 0.0f, 1.0f}, spheres[2], &drawables))
                ->translate(Vector3::yAxis(0.75f))
                ->rotateZ(120.0_degf);
            (new Icosphere(&mesh, &shader, {0.0f, 0.0f, 1.0f}, spheres[2], &drawables))
                ->translate(Vector3::yAxis(0.75f))
                ->rotateZ(240.0_degf);
        }

    protected:
        inline void viewportEvent(const Vector2i& size) {
            defaultFramebuffer.setViewport({{}, size});
            camera->setViewport(size);
        }

        void drawEvent() {
            defaultFramebuffer.clear(DefaultFramebuffer::Clear::Color|DefaultFramebuffer::Clear::Depth);
            camera->draw(drawables);
            swapBuffers();

            cameraObject->rotateX(1.0_degf);
            spheres[0]->rotateZ(-2.0_degf);
            spheres[1]->rotateZ(1.0_degf);
            spheres[2]->rotateZ(-0.5_degf);
            Utility::sleep(40);
            redraw();
        }

    private:
        Scene3D scene;
        SceneGraph::DrawableGroup3D<> drawables;
        Object3D* cameraObject;
        SceneGraph::Camera3D<>* camera;
        Buffer buffer;
        Buffer indexBuffer;
        Mesh mesh;
        PhongShader shader;
        Object3D* spheres[3];
};

}}

MAGNUM_APPLICATION_MAIN(Magnum::Examples::MotionBlurExample)
