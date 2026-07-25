#!/usr/bin/env python3
import sys
from pathlib import Path


def fix_markdown_math(text: str) -> str:
    """
    将 Markdown 中的 LaTeX 公式定界符转换为更常见的 Markdown 写法。

    \\[ ... \\]  ->  $$ ... $$
    \\( ... \\)  ->  $ ... $
    """

    text = text.replace(r"\[", "$$")
    text = text.replace(r"\]", "$$")

    text = text.replace(r"\(", "$")
    text = text.replace(r"\)", "$")

    return text


def main():
    if len(sys.argv) < 2:
        print("用法：python3 fix_md_math.py 文件名")
        sys.exit(1)

    input_path = Path(sys.argv[1])

    if not input_path.exists():
        print(f"错误：文件不存在：{input_path}")
        sys.exit(1)

    if not input_path.is_file():
        print(f"错误：不是普通文件：{input_path}")
        sys.exit(1)

    # 读取原文件
    text = input_path.read_text(encoding="utf-8")

    # 处理内容
    fixed_text = fix_markdown_math(text)

    # 输出到原文件所在目录
    output_path = input_path.with_name(input_path.stem + "_fixed" + input_path.suffix)

    # 写入新文件
    output_path.write_text(fixed_text, encoding="utf-8")

    print(f"处理完成：{output_path}")


if __name__ == "__main__":
    main()