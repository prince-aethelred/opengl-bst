#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

template <typename T>
struct TreeNode {
    T data;
    TreeNode* left;
    TreeNode* right;
};

template <typename T>
std::vector<std::string> to_string_helper(const TreeNode<T>* node) {
    if (node == nullptr) {
        return { "" };
    }
    std::ostringstream oss;
    oss << node->data;
    std::vector<std::string> left_lines = to_string_helper(node->left);
    std::vector<std::string> right_lines = to_string_helper(node->right);
    std::vector<std::string> lines(std::max(left_lines.size(), right_lines.size()));
    std::size_t i = 0;
    for (; i < left_lines.size() && i < right_lines.size(); ++i) {
        lines[i] = left_lines[i] + "   " + right_lines[i];
    }
    for (; i < left_lines.size(); ++i) {
        lines[i] = left_lines[i];
    }
    for (; i < right_lines.size(); ++i) {
        lines[i] = right_lines[i];
    }
    std::size_t width = lines[0].size();
    std::size_t trim_left = 0;
    std::size_t trim_right = width;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::size_t line_width = lines[i].size();
        std::size_t left = (width - line_width) / 2;
        std::size_t right = width - line_width - left;
        lines[i] = std::string(left, ' ') + lines[i] + std::string(right, ' ');
        if (lines[i].find_first_not_of(' ') != std::string::npos) {
            trim_left = std::min(trim_left, lines[i].find_first_not_of(' '));
            trim_right = std::min(trim_right, lines[i].find_last_not_of(' ') + 1);
        }
    }
    std::vector<std::string> result;
    for (const std::string& line : lines) {
        result.push_back(line.substr(trim_left, trim_right - trim_left));
    }
    return result;
}

template <typename T>
std::string to_string(const TreeNode<T>* node) {
    std::vector<std::string> lines = to_string_helper(node);
    std::ostringstream oss;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::string line = lines[i];
        std::size_t j = line.find_first_not_of(' ');
        if (j == std::string::npos) {
            continue;
        }
        std::size_t k = line.find_last_not_of(' ');
        line = line.substr(j, k - j + 1);
        j = line.find(' ');
        while (j != std::string::npos) {
            line.replace(j, 1, node->left != nullptr ? "/" : " ");
            j = line.find(' ', j + 1);
            if (j != std::string::npos) {
                line.replace(j, 1, node->right != nullptr ? "\\" : " ");
                j = line.find(' ', j + 1);
            }
        }
        oss << line << "\n";
    }
    return oss.str();
}

void display();
void reshape(int width, int height);
void draw_node(const TreeNode<int>* node, float x, float y, float radius);
void draw_circle(float x, float y, float radius);

// Create binary search tree
TreeNode<int>* root = new TreeNode<int>{ 5, nullptr, nullptr };

int main(int argc, char** argv) {
    // Initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Binary Search Tree");

    // Initialize OpenGL
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Set projection matrix and viewport
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(45.0f, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 100.0f);
    //glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    // Set model matrix
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    root->left = new TreeNode<int>{ 3, nullptr, nullptr };
    root->right = new TreeNode<int>{ 8, nullptr, nullptr };
    root->left->left = new TreeNode<int>{ 2, nullptr, nullptr };
    root->left->right = new TreeNode<int>{ 4, nullptr, nullptr };
    root->right->left = new TreeNode<int>{ 7, nullptr, nullptr };
    root->right->right = new TreeNode<int>{ 9, nullptr, nullptr };



    // Set display function and start main loop
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();

    return 0;
}

void display() {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    // Draw binary search tree
    draw_node(root, 0.0f, 0.0f, 0.5f);

    // Swap buffers to display scene
    glutSwapBuffers();
}

void reshape(int width, int height) {
    // Set viewport to cover the entire window
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    // Set model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void draw_node(const TreeNode<int>* node, float x, float y, float radius) {
    if (!node) return;

    // Draw circle for current node
    draw_circle(x, y, radius);

    // Draw left child
    if (node->left) {
        float child_x = x - radius * 2;
        float child_y = y - radius * 2;
        draw_node(node->left, child_x, child_y, radius / 2);
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(child_x + radius, child_y + radius);
        glEnd();
    }

    // Draw right child
    if (node->right) {
        float child_x = x + radius * 2;
        float child_y = y - radius * 2;
        draw_node(node->right, child_x, child_y, radius / 2);
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(child_x - radius, child_y + radius);
        glEnd();
    }
}

void draw_circle(float x, float y, float radius) {
    const int num_segments = 32;
    const float theta = 2.0f * 3.1415926f / float(num_segments);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;

    float xx = radius;
    float yy = 0.0f;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);
    for (int i = 0; i < num_segments; i++) {
        glVertex2f(x + xx, y + yy);
        t = xx;
        xx = c * xx - s * yy;
        yy = s * t + c * yy;
    }
    glVertex2f(x + radius, y);
    glEnd();
}
