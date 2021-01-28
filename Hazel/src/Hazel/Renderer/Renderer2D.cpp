#include "Hazel/Renderer/Renderer2D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Debug/Instrumentor.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

    struct QuadVertice
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData
    {
        // Maximum values for a single draw call
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 8;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertice* QuadVerticeBufferBase = nullptr;
        QuadVertice* QuadVerticeBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots>  TextureSlots;
        uint32_t TextureSlotIndex = 1;  // 0 = white texture
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        HZ_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer =  VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertice));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Float, "a_TilingFactor" }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVerticeBufferBase = new QuadVertice[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 3;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("Sandbox/assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Set first texture slot to 0
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
    }

    void Renderer2D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();

        delete[] s_Data.QuadVerticeBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVerticeBufferPtr = s_Data.QuadVerticeBufferBase;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount == 0)
            return;  // Nothing to draw

        uint32_t dataSize = (uint8_t *)s_Data.QuadVerticeBufferPtr - (uint8_t *)s_Data.QuadVerticeBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVerticeBufferBase, dataSize);

        // Bind Textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    }

    // Primitives
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        s_Data.QuadVerticeBufferPtr->Position = { position.x, position.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = color;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x + size.x, position.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = color;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = color;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x, position.y + size.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = color;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadIndexCount += 6;

        // s_Data.TextureShader->SetFloat4("u_Color", color);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        // s_Data.WhiteTexture->Bind();

        // glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
        //     glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
        // s_Data.TextureShader->SetMat4("u_Transform", transform);

        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture , float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        s_Data.QuadVerticeBufferPtr->Position = { position.x, position.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = tintColor;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x + size.x, position.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = tintColor;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = tintColor;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadVerticeBufferPtr->Position = { position.x, position.y + size.y, position.z };
        s_Data.QuadVerticeBufferPtr->Color = tintColor;
        s_Data.QuadVerticeBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVerticeBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVerticeBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVerticeBufferPtr++;

        s_Data.QuadIndexCount += 6;

        // s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        // texture->Bind();

        // glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
        //     glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
        // s_Data.TextureShader->SetMat4("u_Transform", transform);

        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f}, rotation, size, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        s_Data.TextureShader->SetFloat4("u_Color", color);
        s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data.WhiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture , float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f}, rotation, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        HZ_PROFILE_FUNCTION();

        s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

}