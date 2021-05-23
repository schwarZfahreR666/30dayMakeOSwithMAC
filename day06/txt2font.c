// convHankakuTxt.c
// http://bttb.s1.valueserver.jp/wordpress/blog/2017/12/13/makeos-5-1/

#include <stdio.h>
#include <string.h>

int main() {
  FILE *infp;
  FILE *outfp;
  char s1[] = "char hankaku[4096] =  {\n  ";
  char s2[] = "\n};";
  char buf[256] = {0};//与fgets相关,读取每行的最大字节数,char buf[40] = {0}依然可以正常运行
  char wbuf[256] = {0};//256个字符
  int d;
  if ((infp = fopen("hankaku.txt","r")) == NULL) {
    printf("input file open error\n");
    return 0;
  }

  if ((outfp = fopen("hankaku.c","w")) == NULL) {
    printf("output file open error\n");
    return 0;
  }

  fwrite(s1, sizeof(char), strlen(s1), outfp);

  fgets(buf, sizeof(buf), infp);
  printf("buf is : %s",buf);
  for(int k = 0; k < 256; k++) {
    for(int i = 0; i < 2; i++) {
      fgets(buf, sizeof(buf), infp);
    }
    if (k==0)
    {
      printf("buf is : %s",buf);
    }
    
    for(int i = 0; i < 16; i++) {
      d = 0;
      fgets(buf, sizeof(buf), infp);
      for(int j = 0; j < 8; j++) {
        if(buf[j] == '*'){d |= (1 << (8-j-1));}
      }
      sprintf(wbuf, "0x%02x, ", d);
      if(k == 255 && i == 15) {
        fwrite(wbuf, 1, 4, outfp);
      } else {
        fwrite(wbuf, 1, 6, outfp);//额外的两字节是逗号和空格
      }
    }
    fwrite("\n  ", 1, 3, outfp);
  }

  fwrite(s2, sizeof(char), strlen(s2), outfp);
  fclose(infp);
  fclose(outfp);
}