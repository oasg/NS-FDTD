#ifndef MODEL_MODEL_BUILDER_HPP
#define MODEL_MODEL_BUILDER_HPP
#include <iostream>
class ModelBuilder{
public:
    ModelBuilder();
    ModelBuilder(const std::string path);
    ~ModelBuilder();
private:
    std::string path;
};
#endif