#include"frame_buffer_config.hpp"

struct PixelColor {
    uint8_t r, g, b;
};

// PixelWriterインターフェイス
class PixelWriter {
    public:
        // コンストラクタ
        // Write関数を呼び出すたびにconfigを受け取る必要がなくなる。
        PixelWriter(const FrameBufferConfig &config) : config_{config} {
        }

        // デストラクタ
        virtual ~PixelWriter() = default;

        // = 0 → 純粋仮想関数。実装を持たないインターフェイス。
        virtual void Write(int x, int y, const PixelColor &c) = 0;

    protected:
        uint8_t* PixelAt(int x, int y);
    
    private:
        const FrameBufferConfig &config_;
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;
    
        virtual void Write(int x, int y, const PixelColor &c);
};


class BGRResv8BitPerColorPixelWriter: public PixelWriter {
    public:
        using PixelWriter::PixelWriter;

        virtual void Write(int x, int y, const PixelColor &c);
};
