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

    void update();
    std::string read_file_to_string(const std::string& filename) const;
    void draw(OpenGP::Shader& shader, OpenGP::GPUMesh& mesh) const;
};

#endif // RENDERER_H
