#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 描述初始化完成后的返回值 */
const int success = 1;
const int failed = 0;

/* 缓存区大小常量 */
#define buffer_size 1024

/* 存储字体信息的结构体，详细描述可以查看 https://github.com/patorjk/figlet.js/blob/master/doc/figfont.txt */
typedef struct tagFontConfig {
	char hardblank;			// 强制空格
	int height;				// 字体高度
	int baseline;			// 基准线（没用到）
	int max_length;			// 最大宽度
	int old_layout;			// 是否为旧布局（没用到）
	int comment_lines;		// 注释行数
	int print_direction;	// 输出方向（没用到）
	int full_layout;		// 完整布局（未使用）
	int codeag_count;

	char **font_file;		// 按行存储字体内容
	int font_file_lines;	// 字体内容行数，方便回收内存
} fontcfg_t;

/* 存储一行字的结构体 */
typedef struct tagLine {
	char **lines;			// 实际上的一行
	int height;				// 字体高度
	int width;				// 这一行字的宽度
} line_t;

/* 文字对其类型 */
typedef enum { left, center, right } align_t;

const char *defaultFont = "3d_diagonal.flf";	// 默认字体
const char *signature = "flf2a";				// 签名，参考上面的链接

/*
 * 加载字体
 * font_path 为字体路径，
 * cfg 为加载完毕后存储信息的结构体
 */
int load_font(const char *font_path, fontcfg_t *cfg) {
	char header[buffer_size], buffer[buffer_size];
	int i;
	int max_line, readed;
	FILE *font = NULL;

	// 打开文件
	font = fopen(font_path, "r");
	if (!font)
		return failed;

	// 将全部内容读入内存，如果不够就用 realloc 函数扩展 font_file 部分的内存
	for (max_line = 20, readed = 0, cfg->font_file = malloc(sizeof(char *) * 20); !feof(font); ++readed) {
		if (max_line == readed) {
			max_line += 20;
			cfg->font_file = (char **)realloc(cfg->font_file, max_line * sizeof(char *));
		}
		fgets(buffer, buffer_size, font);
		cfg->font_file[readed] = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(cfg->font_file[readed], buffer);
	}
	// 保存总行数
	cfg->font_file_lines = readed;

	// 获取 header 并检查签名
	strcpy(header, cfg->font_file[0]);
	for (i = 0; i != strlen(signature); ++i)
		if (signature[i] != header[i]) {
			return failed;
		}
	// 读取基本信息
	sscanf(header, "flf2a%c %d %d %d %d %d", // %d %d %d",
		&cfg->hardblank,
		&cfg->height,
		&cfg->baseline,
		&cfg->max_length,
		&cfg->old_layout,
		&cfg->comment_lines/*, cfg->print_direction, cfg->full_layout, cfg->codeag_count*/
	);
	return success;
}

/*
 * 渲染一行字，即用 cfg 给定的字体表示 string 中的字符
 */
line_t renderWord(fontcfg_t *cfg, const char *string) {
	line_t line;
	int i, j, offist;

	line.height = cfg->height;
	line.width = 0;

	// 初始化每一行的信息
	line.lines = (char **)malloc(line.height * sizeof(char *));
	for (i = 0; i != line.height; ++i) {
		line.lines[i] = (char *)malloc(cfg->max_length * strlen(string));
		line.lines[i][0] = '\0';
	}

	// 逐个自渲染，每行都渲染到单独的行内
	// 渲染后的字符宽度由 max_length 存储并复制到结构体内
	for (offist = i = 0; i != strlen(string); ++i) {
		int max_length = 0;
		for (j = 0; j != line.height; ++j) {
			if (string[i] - ' ' < 0)
				continue;
			strcat(line.lines[j], cfg->font_file[cfg->comment_lines + (string[i] - ' ') * cfg->height + j + 1]);
			line.lines[j][strlen(line.lines[j]) - 3 - (j == line.height - 1 ? 1 : 0)] = '\0';
			max_length = max(max_length, strlen(line.lines[j]));
		}
		line.width = max(max_length, line.width);
	}

	// 将硬空格还原为普通字符的空格
	for (i = 0; i != line.height; ++i) {
		for (j = 0; line.lines[i][j]; ++j) {
			if (line.lines[i][j] == cfg->hardblank) {
				line.lines[i][j] = ' ';
			}
		}
	}
	return line;
}

/*
* 连接两行字
* 如果 line1 为空，则在 line1 中创建一个 line2 的副本
*/
void linecat(line_t *line1, line_t *line2) {
	int i;
	if (line1->height != line2->height) {
		// line1 为空，拷贝
		line1->height = line2->height;
		line1->lines = (char **)malloc(line2->height * sizeof(char *));
		for (i = 0; i != line2->height; ++i) {
			line1->lines[i] = malloc((strlen(line2->lines[i]) + 2) * sizeof(char));
			strcpy(line1->lines[i], line2->lines[i]);
			line1->width = line2->width;
		}
		return;
	}
	// line1 非空，连接
	for (i = 0; i != line1->height; ++i) {
		int pos = strlen(line1->lines[i]);
		line1->lines[i] = realloc(line1->lines[i], (strlen(line1->lines[i]) + strlen(line2->lines[i]) + 2) * sizeof(char));
		line1->lines[i][pos] = '\0';
		strcat(line1->lines[i], line2->lines[i]);
		line1->width = max(line1->width, strlen(line1->lines[i]));
	}
}

// 释放字体信息所占用的资源
void free_font(fontcfg_t *cfg) {
	int i;
	for (i = 0; i != cfg->font_file_lines; ++i)
		free(cfg->font_file[i]);
	free(cfg->font_file);
}

// 释放一行字所占用的资源
void free_line(line_t line) {
	int i;
	if (line.width == 0) return;
	for (i = 0; i != line.height; ++i)
		free(line.lines[i]);
	free(line.lines);
}

// 输出 cnt 个空格
void print_space(int cnt) {
	while (cnt--) putchar(' ');
}

// 将 line 中的内容按照对齐方式 align 输出值屏幕，输出最大宽度由 target_width 约束
void print_line(align_t align, int target_width, line_t line) {
	int spaces = 0, i;
	switch (align) {
	default:
	case left:
		break;
	case right:
		spaces = target_width - line.width - 1;
		break;
	case center:
		spaces = (target_width - line.width - 1) / 2;
		break;
	}
	for (i = 0; i != line.height; ++i) {
		print_space(spaces);
		puts(line.lines[i]);
	}
}

int main(int argc, char *argv[]) {
	fontcfg_t fontcfg;
	int target_width = 80;
	align_t align;
	int i;
	char in[buffer_size];
	char *font = defaultFont;

	// 分析所有的命令行参数
	for (i = 1; i != argc; ++i) {
		if (strcmp(argv[i], "-c") == 0) {		// 对齐方式：居中
			align = center;
		}
		else if (strcmp(argv[i], "-l") == 0) {	// 对齐方式：靠左
			align = left;
		}
		else if (strcmp(argv[i], "-r") == 0) {	// 对齐方式：靠右
			align = right;
		}
		else if (strcmp(argv[i], "-w") == 0) {	// 指定输出宽度
			i++;
			sscanf(argv[i], "%d", &target_width);
		}
		else if (strcmp(argv[i], "-f") == 0) {	// 指定字体文件 
			// 由于解压zip 代码难以实现，需要提前用winrar之类的工具将字体解压。
			// 例如字体 3-d.flf，你需要将 3-d.flf 重命名为 3-d.zip 并打开，将里面的3-d.f
			// lf 解压到工程/可执行文件目录，才能使用
			i++;
			font = argv[i];
		}
	}

	// 尝试载入字体
	if (!load_font(font, &fontcfg)) {
		printf("Cannot load font file.");
		return EXIT_FAILURE;
	}

	for (;;) {	// 主循环，反复读取输入直到用户输入 quit
		line_t line, word, ch;

		fgets(in, buffer_size, stdin);
		in[strlen(in) - 1] = '\0';
		line.lines = word.lines = NULL;
		line.width = word.width = 0;
		line.height = word.height = 0;

		for (i = 0; i != strlen(in); ++i) {
			char str[20];
			str[0] = in[i], str[1] = '\0';
			ch = renderWord(&fontcfg, str);
			/*
			 * 渲染思路：
			 *     每次渲染一个字符，然后检查将这个字符和单词连接起来是否会超出指定的范围，
			 * 如果是，则先将已渲染的单词内容打印到屏幕，来防止在字母中间隔开的情况。如果顺
			 * 利渲染完一个单词，就检查如果将这个单词放到句子中是否会超长。如果是则先输出句
			 * 子中的内容，否则就将这个单词连接到句子末尾，并准备下一个单词。
			 *     从中不难看出，当单个单词超过屏幕范围时，需要提前将已放置在句子中的内容输
			 * 出到屏幕，来保证顺序不出现错误。
			 */
			if (word.width + ch.width >= target_width - 1) {	// 检查这个字符和单词连接起来是否会超出范围
				if (line.width != 0) {							// 已经有句子没输出，需要先处理
					print_line(align, target_width, line);
					line.width = line.height = 0;
				}
				print_line(align, target_width, word);
				free_line(word);
				word.width = word.height = 0;
			}
			linecat(&word, &ch);	// 将字母连接到单子末尾

			if (in[i] == ' ' && word.width > 1) {	// 空格就是单词的结束，检查连接到句尾会不会超长
				if (line.width + word.width >= target_width - 1) {
					print_line(align, target_width, line);
					line.width = line.height = 0;
				}
				linecat(&line, &word);
				free_line(word);
				word.width = word.height = 0;
			}
		}
		// 将残留在缓存的单子和句子中的内容输出
		if (line.height && line.width + word.width >= target_width - 1) {
			print_line(align, target_width, line);
			line.width = line.height = 0;
		}
		linecat(&line, &word);
		word.width = word.height = 0;
		print_line(align, target_width, line);
		// 检查是否要退出
		if (strcmp(in, "quit") == 0) break;
	}
	// 释放申请的资源，并退出
	free_font(&fontcfg);
	return EXIT_SUCCESS;
}