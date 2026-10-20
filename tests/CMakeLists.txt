
# 生成测试可执行程序
add_executable(test_tensor
    test_tensor.cpp
)

# 链接 MiniTensor 静态库
target_link_libraries(test_tensor
    PRIVATE
        minitensor
)

# 向 CTest 注册测试
add_test(
    NAME TensorTest
    COMMAND test_tensor
)
