// ------------------------------------------------------------
// Copyright(c) 2018-2022 Jesse Yurkovich
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// ------------------------------------------------------------
#pragma once

#include <fx/gltf.h>
#include "MaterialData.h"

class MeshData
{
public:
    struct BufferInfo
    {
        fx::gltf::Accessor const * Accessor;

        uint8_t const * Data;
        uint64_t DataStride;
        uint64_t TotalSize;
        uint16_t DataType;
        uint64_t elementSize;

        bool HasData() const noexcept
        {
            return Data != nullptr;
        }
    };

    MeshData(fx::gltf::Document const & doc, std::size_t meshIndex, std::size_t primitveIndex)
    {
        fx::gltf::Mesh const & mesh = doc.meshes[meshIndex];
        fx::gltf::Primitive const & primitive = mesh.primitives[primitveIndex];

        if(primitive.attributes.empty()){
            printf("attributes of this primitive is empty\n");
        }

        for (auto const & attrib : primitive.attributes)
        {
            if (attrib.first == "POSITION")
            {
                printf("vertexBuffer:");
                m_vertexBuffer = GetData(doc, doc.accessors[attrib.second]);
            }
            else if (attrib.first == "NORMAL")
            {
                printf("normalBuffer:");
                m_normalBuffer = GetData(doc, doc.accessors[attrib.second]);
            }
            else if (attrib.first == "TANGENT")
            {
                printf("tangentBuffer:");
                m_tangentBuffer = GetData(doc, doc.accessors[attrib.second]);
            }
            else if (attrib.first == "TEXCOORD_0")
            {
                printf("TextureBuffer:");
                m_texCoord0Buffer = GetData(doc, doc.accessors[attrib.second]);
            }
        }
        if(primitive.indices >= 0){
            m_indexBuffer = GetData(doc, doc.accessors[primitive.indices]);
        }

        if (primitive.material >= 0)
        {
            m_materialData.SetData(doc.materials[primitive.material]);
        }
    }

    BufferInfo const & IndexBuffer() const noexcept
    {
        return m_indexBuffer;
    }

    BufferInfo const & VertexBuffer() const noexcept
    {
        return m_vertexBuffer;
    }

    BufferInfo const & NormalBuffer() const noexcept
    {
        return m_normalBuffer;
    }

    BufferInfo const & TangentBuffer() const noexcept
    {
        return m_tangentBuffer;
    }

    BufferInfo const & TexCoord0Buffer() const noexcept
    {
        return m_texCoord0Buffer;
    }

    MaterialData const & Material() const noexcept
    {
        return m_materialData;
    }

private:
    BufferInfo m_indexBuffer{};
    BufferInfo m_vertexBuffer{};
    BufferInfo m_normalBuffer{};
    BufferInfo m_tangentBuffer{};
    BufferInfo m_texCoord0Buffer{};

    MaterialData m_materialData{};

    static BufferInfo GetData(fx::gltf::Document const & doc, fx::gltf::Accessor const & accessor)
    {
        fx::gltf::BufferView const & bufferView = doc.bufferViews[accessor.bufferView];
        fx::gltf::Buffer const & buffer = doc.buffers[bufferView.buffer];

        const uint64_t dataTypeSize = CalculateDataTypeSize(accessor);
        const uint16_t dataTypeGL = CalculateDataType(accessor);
        const uint64_t elementSize = CalculateElementSize(accessor);
        return BufferInfo{ &accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + static_cast<uint64_t>(accessor.byteOffset)], dataTypeSize, static_cast<uint64_t>(accessor.count) * static_cast<uint64_t>(dataTypeSize), dataTypeGL, elementSize};
    }

    static uint16_t CalculateDataType(fx::gltf::Accessor const & accessor) noexcept{
        uint16_t elementSize = 0;
        switch (accessor.componentType)
        {
        case fx::gltf::Accessor::ComponentType::Byte:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::Byte;
            break;
        case fx::gltf::Accessor::ComponentType::UnsignedByte:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedByte;
            break;
        case fx::gltf::Accessor::ComponentType::Short:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::Short;
            break;
        case fx::gltf::Accessor::ComponentType::UnsignedShort:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedShort;
            break;
        case fx::gltf::Accessor::ComponentType::Float:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::Float;
            break;
        case fx::gltf::Accessor::ComponentType::UnsignedInt:
            elementSize = (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedInt;
            break;
        }
        return elementSize;
    }

    static uint64_t CalculateElementSize(fx::gltf::Accessor const & accessor) noexcept{
        uint64_t elementSize = 0;
        switch (accessor.componentType)
        {
        case fx::gltf::Accessor::ComponentType::Byte:
        case fx::gltf::Accessor::ComponentType::UnsignedByte:
            elementSize = 1;
            break;
        case fx::gltf::Accessor::ComponentType::Short:
        case fx::gltf::Accessor::ComponentType::UnsignedShort:
            elementSize = 2;
            break;
        case fx::gltf::Accessor::ComponentType::Float:
        case fx::gltf::Accessor::ComponentType::UnsignedInt:
            elementSize = 4;
            break;
        }
        return elementSize;
    }

    static uint64_t CalculateDataTypeSize(fx::gltf::Accessor const & accessor) noexcept
    {
        uint64_t elementSize = 0;
        switch (accessor.componentType)
        {
        case fx::gltf::Accessor::ComponentType::Byte:
        case fx::gltf::Accessor::ComponentType::UnsignedByte:
            elementSize = 1;
            break;
        case fx::gltf::Accessor::ComponentType::Short:
        case fx::gltf::Accessor::ComponentType::UnsignedShort:
            elementSize = 2;
            break;
        case fx::gltf::Accessor::ComponentType::Float:
        case fx::gltf::Accessor::ComponentType::UnsignedInt:
            elementSize = 4;
            break;
        }

        switch (accessor.type)
        {
        case fx::gltf::Accessor::Type::Mat2:
            return 4 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Mat3:
            return 9 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Mat4:
            return 16 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Scalar:
            return elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec2:
            return 2 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec3:
            return 3 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec4:
            return 4 * elementSize;
            break;
        }

        return 0;
    }
};
