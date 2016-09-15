#FLF 字体

基本都参考的这个说明：[figfont.txt](https://github.com/patorjk/figlet.js/blob/master/doc/figfont.txt)

对面要求很低，所以没有必要实现完整的功能。所以只能做到简单的转化和单词换行避免切断。

标准的 flf 字体格式是 zip 压缩过的，由于对面要求很低，所以解压缩工作并不需要用代码完成，同时也就说明运行时需要带上解压后的文件。

使用 -l, -c, -r 来指定左/中/右对齐，使用 -w 指定输出宽度，使用 -f 指定模板文件。
