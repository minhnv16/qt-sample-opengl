
namespace l3_textures{
    int main_1texture();
    int main_2texture();
    int main_2texture_2shader_wrapper();

}
namespace l3_textures_1renderer_performance{
    int main_single_renderer_2texture_multi_object();
}
int main(){
    l3_textures::main_1texture();
    l3_textures::main_2texture();
    l3_textures::main_2texture_2shader_wrapper();


    l3_textures_1renderer_performance::main_single_renderer_2texture_multi_object();

}
