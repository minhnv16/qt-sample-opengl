
namespace l1{
    int main();
}
namespace l1_refact{
    int main();
    int main_indices();

}
namespace l1_ex{
    int main();
}
namespace l1_ex2{
    int main();
}

namespace l1_ex2_2vao_2vbo{
    int main();

}
namespace l1_ex3_2shader_program{
    int main();
}
int main(){
    //l1::main();  //drawing triangle by array
    //l1_refact::main();  //drawing triangle by array
    l1_refact::main_indices();  //drawing triangle by incides

    //l1_ex::main();   //drawing triangle by incides
    //l1_ex2::main();   //1 program, 2 VAOs, VBOs
    //l1_ex2_2vao_2vbo::main();//1 program, 2 VAOs, VBOs sample

    //l1_ex3_2shader_program::main();//2 program, 2 VAOs, VBOs
}
