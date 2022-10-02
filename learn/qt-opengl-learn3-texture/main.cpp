
namespace l3_textures{
    int main_1texture();
    int main_2texture();
    int main_2texture_2shader_wrapper();

}
namespace l3_textures_1renderer{
    int main_1renderer_2texture_multi_object();
}
namespace l3_textures_1renderer_optimize_performance{
    int main_1renderer_2texture_multi_object_optimize();
}
int main(){
//    l3_textures::main_1texture();
//    l3_textures::main_2texture();
//    l3_textures::main_2texture_2shader_wrapper();


    l3_textures_1renderer::main_1renderer_2texture_multi_object();
    l3_textures_1renderer_optimize_performance::main_1renderer_2texture_multi_object_optimize();
}
