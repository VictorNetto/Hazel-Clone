#include <Hazel.h>

#include <string>
#include <memory>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Hazel::Ref<Hazel::Shader> shader;
Hazel::Ref<Hazel::VertexArray> vertexArray;

Hazel::Ref<Hazel::Shader> flatColorShader, textureShader;
Hazel::Ref<Hazel::VertexArray> squareVA;

Hazel::Ref<Hazel::Texture2D> texture;
Hazel::Ref<Hazel::Texture2D> chernoLogoTexture;

Hazel::OrthographicCamera camera(-1.2f, 1.2f, -0.9f, 0.9f);
glm::vec2 cameraPosition = { 0.0f, 0.0f };
float cameraRotation = 0.0f;
float cameraSpeed = 1.0f;
float cameraRotationSpeed = 45.0f;

glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };

void InitVAs()
{
    vertexArray.reset(Hazel::VertexArray::Create());

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

    Hazel::BufferLayout layout = {
        { Hazel::ShaderDataType::Float3, "a_Position" },
        { Hazel::ShaderDataType::Float4, "a_Color" }
    };
    vertexBuffer->SetLayout(layout);

    uint32_t indices[3] = { 0, 1, 2 };
    Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
    indexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));

    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);


    squareVA.reset(Hazel::VertexArray::Create());

    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVB->SetLayout({
        { Hazel::ShaderDataType::Float3, "a_Position" },
        { Hazel::ShaderDataType::Float2, "a_TexCoord" }
    });

    uint32_t squareIndices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    Hazel::Ref<Hazel::IndexBuffer> squareIB;
    squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, 6));

    squareVA->AddVertexBuffer(squareVB);
    squareVA->SetIndexBuffer(squareIB);
}

void InitShaders()
{
    std::string vertexSrc = R"(
        #version 450 core

        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    std::string fragmentSrc = R"(
        #version 450 core

        layout(location = 0) out vec4 color;

        in vec4 v_Color;

        void main()
        {
            color = v_Color;
        }
    )";

    std::string flatColorShaderVertexSrc = R"(
        #version 450 core

        layout (location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main()
        {
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    std::string flatColorShaderFragmentSrc = R"(
        #version 450 core

        layout(location = 0) out vec4 color;

        uniform vec3 u_Color;

        void main()
        {
            color = vec4(u_Color, 1.0);
        }
    )";

    shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));
    flatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
    textureShader.reset(Hazel::Shader::Create("Sandbox/assets/shaders/Texture.glsl"));

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

void InitTextures()
{
    texture = Hazel::Texture2D::Create("Sandbox/assets/textures/Checkerboard.png");
    chernoLogoTexture = Hazel::Texture2D::Create("Sandbox/assets/textures/ChernoLogo.png");
}

void InitCamera()
{
}

class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer()
        : Layer("Example")
    {
    }
    
    void OnUpdate(Hazel::Timestep ts) override
    {
        int framesToAverage = 10;
        m_FrameRate = (framesToAverage - 1) * m_FrameRate + 1000.0f / (ts.GetMilisecond());
        m_FrameRate /= framesToAverage;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP) || Hazel::Input::IsKeyPressed(HZ_KEY_W))
            cameraPosition.y += cameraSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN) || Hazel::Input::IsKeyPressed(HZ_KEY_S))
            cameraPosition.y -= cameraSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT) || Hazel::Input::IsKeyPressed(HZ_KEY_A))
            cameraPosition.x -= cameraSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT) || Hazel::Input::IsKeyPressed(HZ_KEY_D))
            cameraPosition.x += cameraSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
            cameraRotation += cameraRotationSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
            cameraRotation -= cameraRotationSpeed * ts;
        
        camera.SetRotation(cameraRotation);
        camera.SetPosition({ cameraPosition.x, cameraPosition.y, 0.0f });

        Hazel::Renderer::BeginScene(camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", squareColor);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

                Hazel::Renderer::Submit(flatColorShader, squareVA, transform);
            }
        }

        texture->Bind();
        Hazel::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        chernoLogoTexture->Bind();
        Hazel::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Triangle
        // Hazel::Renderer::Submit(shader, vertexArray);

        Hazel::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Info");
        ImGui::Text("Frame Rate: %.2f", m_FrameRate);
        ImGui::Text("Renderer API: %s", Hazel::RendererInfo::GetRendererAPI().c_str());
        ImGui::Text("Vendor: %s", Hazel::RendererInfo::GetVendor().c_str());
        ImGui::Text("Renderer: %s", Hazel::RendererInfo::GetRenderer().c_str());
        ImGui::Text("Version: %s", Hazel::RendererInfo::GetVersion().c_str());
        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
        ImGui::End();
    }

    void OnEvent(Hazel::Event& event) override
    {
        // HZ_CLIENT_TRACE("{0}", event);
    }
private:
    float m_FrameRate = 60.0f;
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());

        InitVAs();
        InitShaders();
        InitCamera();
        InitTextures();
    }

    ~Sandbox()
    {
    }
};

Hazel::Application * Hazel::CreateApplication()
{
    return new Sandbox;
}