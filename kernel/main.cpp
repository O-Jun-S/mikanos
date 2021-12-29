#include<cstdint>
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
        uint8_t* PixelAt(int x, int y) {
            return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
        }
    
    private:
        const FrameBufferConfig &config_;
};


class RGBResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;
    
        virtual void Write(int x, int y, const PixelColor &c) override {
            auto p = PixelAt(x, y);
            p[0] = c.r;
            p[1] = c.g;
            p[2] = c.b;
        }
};


class BGRResv8BitPerColorPixelWriter: public PixelWriter {
    public:
        using PixelWriter::PixelWriter;

        virtual void Write(int x, int y, const PixelColor &c) override {
            auto p = PixelAt(x, y);
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
        }
};


/** WritePixel draw one point.
 * @retval 0        success
 * @retval not 0    failed
 */
int WritePixel(
    const FrameBufferConfig& config,
    int x, int y, const PixelColor& c
) {
    const int pixel_position = config.pixels_per_scan_line * y + x;
    if(config.pixel_format == kPixelRGBResv8BitPerColor) {
        uint8_t* p = &config.frame_buffer[4 * pixel_position];
        p[0] = c.r;
        p[1] = c.g;
        p[2] = c.b;
    }

    else if (config.pixel_format == kPixelBGRResv8BitPerColor) {
        uint8_t* p = &config.frame_buffer[4 * pixel_position];
        p[0] = c.b;
        p[1] = c.g;
        p[2] = c.r;
    }

    else {
        return -1;
    }

    return 0;
}

extern "C" void KernelMain(
    FrameBufferConfig& frame_buffer_config
) {
    for(int x=0; x<frame_buffer_config.horizontal_resolution; x++) {
        for(int y=0; y<frame_buffer_config.vertical_resolution; y++) {
            WritePixel(frame_buffer_config, x, y, {255, 255, 255});
        }

        for(int x=0; x<200; x++) {
            for(int y=0; y<100; y++) {
                WritePixel(frame_buffer_config, 100+x, 100+y, {0, 100, 100});
            }
        }
    }
    while(1) __asm__("hlt");
}
