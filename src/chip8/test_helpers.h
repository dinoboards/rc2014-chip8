
bool testFailure = false;
bool appRunning = false;

#define assert(a)                            \
  {                                          \
    xprintf(#a "\r\n");                      \
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
