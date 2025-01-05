//
//
#include <QString>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QDataStream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Volcano/Resource/Model.h>
#include <Volcano/ModelCompiler/Common.h>
#include <Volcano/ModelCompiler/IOSystem.h>

VOLCANO_MODELCOMPILER_BEGIN

static QJsonObject dumpMesh(const aiMesh* mesh) {
    QJsonObject obj;
    obj["name"] = mesh->mName.C_Str();
    obj["vertexCount"] = int(mesh->mNumVertices);
    obj["hasNormals"] = mesh->HasNormals();
    obj["colorChannelCount"] = int(mesh->GetNumColorChannels());
    obj["uvChannelCount"] = int(mesh->GetNumUVChannels());
    return obj;
}

static QJsonObject dumpMaterial(const aiMaterial* material) {
    QJsonObject obj;
    obj["name"] = material->GetName().C_Str();
    QJsonArray properties;
    for (int i = 0; i < material->mNumProperties; ++i) {
        QJsonObject prop;
        const aiMaterialProperty* p = material->mProperties[i];
        prop[p->mKey.C_Str()] = "<>";
        properties.append(prop);
    }
    if (!properties.isEmpty()) {
        obj["properties"] = properties;
    }
    return obj;
}

static QJsonObject dumpTexture(const aiTexture* texture) {
    QJsonObject obj;
    obj["path"] = texture->mFilename.C_Str();
    obj["width"] = int(texture->mWidth);
    obj["height"] = int(texture->mHeight);
    return obj;
}

static QJsonObject dumpCamera(const aiCamera* camera) {
    QJsonObject obj;
    obj["name"] = camera->mName.C_Str();
    return obj;
}

static QJsonObject dumpAnimation(const aiAnimation* animation) {
    QJsonObject obj;
    obj["name"] = animation->mName.C_Str();
    return obj;
}

static QJsonObject dumpLight(const aiLight* light) {
    QJsonObject obj;
    obj["name"] = light->mName.C_Str();
    return obj;
}

static QJsonObject dumpScene(const aiScene* scene) {
    QJsonObject obj;

    if (scene->HasMeshes()) {
        QJsonArray meshes;
        for (int i = 0; i < scene->mNumMeshes; ++i) {
            meshes.append(dumpMesh(scene->mMeshes[i]));
        }
        obj["meshes"] = meshes;
    }

    if (scene->HasMaterials()) {
        QJsonArray materials;
        for (int i = 0; i < scene->mNumMaterials; ++i) {
            materials.append(dumpMaterial(scene->mMaterials[i]));
        }
        obj["materials"] = materials;
    }

    if (scene->HasTextures()) {
        QJsonArray textures;
        for (int i = 0; i < scene->mNumTextures; ++i) {
            textures.append(dumpTexture(scene->mTextures[i]));
        }
        obj["textures"] = textures;
    }

    if (scene->HasCameras()) {
        QJsonArray cameras;
        for (int i = 0; i < scene->mNumCameras; ++i) {
            cameras.append(dumpCamera(scene->mCameras[i]));
        }
        obj["cameras"] = cameras;
    }

    if (scene->HasAnimations()) {
        QJsonArray animations;
        for (int i = 0; i < scene->mNumAnimations; ++i) {
            animations.append(dumpAnimation(scene->mAnimations[i]));
        }
        obj["animations"] = animations;
    }

    if (scene->HasLights()) {
        QJsonArray lights;
        for (int i = 0; i < scene->mNumLights; ++i) {
            lights.append(dumpLight(scene->mLights[i]));
        }
        obj["lights"] = lights;
    }

    return obj;
}

static bool compile(const aiScene* scene, const aiMesh* node, QIODevice* output) {
    Resource::Model model;

    //model.mesh_list.append()

    QDataStream output_stream(output);
    output_stream << model;

    return true;
}

static int run(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("VolcanoModelCompiler");
    app.setApplicationVersion(VOLCANO_VERSION_STR);

    QCommandLineParser cmdline;
    cmdline.addOptions({
        { { "d", "dump" }, "Dump scene." },
        { { "m", "mesh" }, "Specify the import mesh.", "meshName" },
        { { "o", "output" }, "Specify the output filename.", "outputFilename" }
    });

    cmdline.addPositionalArgument("inputFilename", "Specify the input model filename.");
    cmdline.addHelpOption();
    cmdline.addVersionOption();

    cmdline.process(app);

    auto input_filenames = cmdline.positionalArguments();
    if (input_filenames.size() != 1) {
        error() << "Only 1 input model filename.";
        return EXIT_FAILURE;
    }

    auto input_filename = input_filenames.at(0);
    auto output_filename = cmdline.value("output");

    if (input_filename.isEmpty()) {
        error() << "Invalid input filename.";
        return EXIT_FAILURE;
    }

    if (!output_filename.isEmpty()) {
        QFileInfo fi(output_filename);
        if (fi.suffix() != "vmodel") {
            output_filename += ".vmodel";
        }
    } else {
        QFileInfo fi(input_filename);
        output_filename = fi.baseName() + ".vmodel";
    }

    Assimp::Importer importer;

    auto scene = importer.ReadFile(qPrintable(input_filename), 0);
    if (scene == nullptr) {
        error() << "Cannot read input model file: " << input_filename;
        return EXIT_FAILURE;
    }

    aiMesh* mesh = nullptr;
    auto mesh_name = cmdline.value("mesh");
    if (!mesh_name.isEmpty()) {
        for (int i = 0; i < scene->mNumMeshes; ++i) {
            if (mesh_name == scene->mMeshes[i]->mName.C_Str()) {
                mesh = scene->mMeshes[i];
            }
        }
        if (mesh == nullptr) {
            error() << "Mesh '" << mesh_name << "' is not found.";
            return EXIT_FAILURE;
        }
    }

    if (cmdline.isSet("dump")) {
        if (mesh != nullptr) {
            QJsonDocument json(dumpMesh(mesh));
            info() << json.toJson();
        } else {
            QJsonDocument json(dumpScene(scene));
            info() << json.toJson();
        }
        return EXIT_SUCCESS;
    }

    if (mesh == nullptr) {
        if (scene->mNumMeshes == 0) {
            error() << "No available mesh.";
            return EXIT_FAILURE;
        }
        warning() << "Mesh is not specified, use the first one.";
        mesh = scene->mMeshes[0];
    }

    QFile output(output_filename);
    if (!output.open(QFile::WriteOnly | QFile::Truncate)) {
        error() << "Failed to create output file " << output_filename;
        return EXIT_FAILURE;
    }

    info() << "Compiling " << input_filename << " [" << mesh->mName.C_Str() << "] to " << output_filename << " ...";
    return compile(scene, mesh, &output) ? EXIT_SUCCESS : EXIT_FAILURE;
}

VOLCANO_MODELCOMPILER_END

int main(int argc, char* argv[]) {
    return Volcano::ModelCompiler::run(argc, argv);
}
