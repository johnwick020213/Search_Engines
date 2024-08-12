#!/usr/bin/env python
# coding=utf-8
import os
import glob

def generate_implementation(header_file):
    impl_file = header_file.replace('.h', '.cpp')

    # 检查实现文件是否已经存在
    if os.path.exists(impl_file):
        print("Skipped: {} already exists.".format(impl_file))
        return
    
    with open(header_file, 'r') as f:
        lines = f.readlines()

    with open(impl_file, 'w') as f:
        # 生成包含头文件的 include 语句
        f.write('#include "{}"\n\n'.format(header_file))
        
        for line in lines:
            if line.strip().endswith(';') and '(' in line and ')' in line:
                func_impl = line.strip().replace(';', ' {\n\n}\n')
                if 'class ' in func_impl:
                    func_impl = func_impl.replace('class ', '')
                f.write(func_impl + '\n')

    print("Generated implementation file: {}".format(impl_file))

if __name__ == '__main__':
    # 使用 glob 模块查找当前目录下所有 .h 文件
    header_files = glob.glob('*.h')
    
    if not header_files:
        print("No header files found in the current directory.")
    else:
        for header_file in header_files:
            generate_implementation(header_file)

