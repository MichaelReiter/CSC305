#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height);

    ~Renderer();

    int create_application() const;

private:
    unsigned int m_width;
    unsigned int m_height;

    void draw() const;
};

#endif // RENDERER_H
