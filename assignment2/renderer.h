#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height);

    ~Renderer();

    int create_application();

private:
    float m_time;
    unsigned int m_width;
    unsigned int m_height;
    std::unique_ptr<OpenGP::Shader> m_shader;
    std::unique_ptr<OpenGP::GPUMesh> m_mesh;

    void draw();
    void update();
    std::string load_source(const std::string& filename) const;
};

#endif // RENDERER_H
