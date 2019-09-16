void ustrcpy(unsigned char* str1, unsigned char* str2) {
  int i = 0;
  while (str2[i] != '\0') {
    str1[i] = str2[i];
    i++;
  }
  str1[i] = str2[i];
}

int ustrlen(unsigned char* str) {
  int i = 0;
  while (str[i] != '\0') {
    i++;
  }
  i++;
  return i;
}
