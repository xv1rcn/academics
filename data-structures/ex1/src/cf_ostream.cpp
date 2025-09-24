#include <iostream>

class cf_ostream : public std::ostream {
    class cf_streambuf : public std::streambuf {
        std::streambuf *buf1, *buf2;

    public:
        cf_streambuf(std::streambuf *b1, std::streambuf *b2) : buf1(b1), buf2(b2) {}
        int overflow(int c) override {
            if (c == EOF) return !EOF;
            if (buf1->sputc(c) == EOF) return EOF;
            if (buf2->sputc(c) == EOF) return EOF;
            return c;
        }
        int sync() override {
            int r1 = buf1->pubsync();
            int r2 = buf2->pubsync();
            return (r1 == 0 && r2 == 0) ? 0 : -1;
        }
    };
    cf_streambuf buf;

public:
    cf_ostream(std::ostream &o1, std::ostream &o2) : std::ostream(&buf), buf(o1.rdbuf(), o2.rdbuf()) {}
};
