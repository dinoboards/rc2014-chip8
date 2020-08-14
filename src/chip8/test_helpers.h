
bool testFailure = false;
bool appRunning = false;

#define assert(a)                            \
  {                                          \
    xprintf(#a "\r\n");                      \
    resetCaptureCommands();                  \
    initSystemState();                       \
    setup_##a();                             \
    appRunning = executeSingleInstruction(); \
    verify_##a();                            \
    xprintf("\r\n");                         \
  }

void expectFalse(bool a, const char *msg) {
  if (!a) {
    xprintf("\t%s == false\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to be false but was %d\r\n", msg, (int)a);
}

void expectTrue(bool a, const char *msg) {
  if (a) {
    xprintf("\t%s == true\r\n", msg);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to be true but was %d\r\n", msg, (int)a);
}

void expectEqualBytes(byte a, byte b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualInts(int a, int b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %d\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %d, but was %d\r\n", msg, b, a);
}

void expectEqualPtrs(uint16_t *a, uint16_t *b, const char *msg) {
  if (a == b) {
    xprintf("\t%s == %p\r\n", msg, a);
    return;
  }

  testFailure = true;
  xprintf("Expected %s to equal %p, but was %p\r\n", msg, b, a);
}

void replace(char *string, char from, char to) {
  for (int i = 0; i < strlen(string); ++i)
    if (string[i] == from)
      string[i] = to;
}

void unescape(const char *string) { replace((char *)string, '\033', '~'); }

void expectEqualEscapedString(const char *a, const char *b) {
  if (strcmp(a, b) == 0) {
    unescape(a);
    xprintf("\tSent: \"%s\"\r\n", a);
    return;
  }

  testFailure = true;

  unescape(a);
  unescape(b);

  xprintf("Expected output of \"%s\" but got \"%s\"\r\n", b, a);
}

#define MAX_CAPTURE_TEXT 256 * 10
char buffer[MAX_CAPTURE_TEXT];

void resetCaptureCommands() { buffer[0] = '\0'; }

void sendDrawCommands(const char *msg, ...) {
  va_list arg;
  va_start(arg, msg);
  char *pBuffer = buffer + strlen(buffer);
  int   max = MAX_CAPTURE_TEXT - strlen(buffer);
  vsnprintf(pBuffer, max - 1, (char *)msg, arg);
  va_end(arg);
}
