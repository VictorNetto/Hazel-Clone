#include <Hazel.h>

#include <string>
#include <memory>

std::shared_ptr<Hazel::Shader> shader;
std::shared_ptr<Hazel::VertexArray> vertexArray;

std::shared_ptr<Hazel::Shader> blueShader;
std::shared_ptr<Hazel::VertexArray> squareVA;

void InitVAs()
{
    vertexArray.reset(Hazel::VertexArray::Create());

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

    Hazel::BufferLayout layout = {
        { Hazel::ShaderDataType::Float3, "a_Position" },
        { Hazel::ShaderDataType::Float4, "a_Color" }
    };
    vertexBuffer->SetLayout(layout);

    uint32_t indices[3] = { 0, 1, 2 };
    std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
    indexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));

    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);


    squareVA.reset(Hazel::VertexArray::Create());

    float squareVertices[3 * 4] = {
        -0.75f, -0.75f, 0.0f,
         0.75f, -0.75f, 0.0f,
         0.75f,  0.75f, 0.0f,
        -0.75f,  0.75f, 0.0f
    };
    std::shared_ptr<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    squareVB->SetLayout({
        { Hazel::ShaderDataType::Float3, "a_Position" }
    });

    uint32_t squareIndices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    std::shared_ptr<Hazel::IndexBuffer> squareIB;
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

        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = vec4(a_Position, 1.0);
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

    std::string blueShaderVertexSrc = R"(
        #version 450 core

        layout (location = 0) in vec3 a_Position;

        void main()
        {
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string blueShaderFragmentSrc = R"(
        #version 450 core

        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(0.2, 0.3, 0.8, 1.0);
        }
    )";

    shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
    blueShader.reset(new Hazel::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
}

class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer()
        : Layer("Example") {}
    
    void OnUpdate() override
    {
        Hazel::Renderer::BeginScene();

        blueShader->Bind();
        Hazel::Renderer::Submit(squareVA);

        shader->Bind();
        Hazel::Renderer::Submit(vertexArray);

        Hazel::Renderer::EndScene();
    }

    void OnEvent(Hazel::Event& event) override
    {
        // HZ_CLIENT_TRACE("{0}", event);
    }
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());

        InitVAs();
        InitShaders();
    }

    ~Sandbox()
    {
    }
};

Hazel::Application * Hazel::CreateApplication()
{
    return new Sandbox;
}