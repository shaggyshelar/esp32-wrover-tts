#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "esp_system.h"

#include "flite.h"

#define SAMPLE_RATE     (44100)
#define I2S_NUM         (0)
#define WAVE_FREQ_HZ    (100)
#define PI              (3.14159265)
#define I2S_BCK_IO      (GPIO_NUM_4)
#define I2S_WS_IO       (GPIO_NUM_5)
#define I2S_DO_IO       (GPIO_NUM_18)
#define I2S_DI_IO       (-1)
#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)

cst_voice *register_cmu_us_kal(const char *voxdir);
cst_voice *v;
// cst_voice *v = register_cmu_us_kal(NULL);

int i2s_stream_chunk(const cst_wave *w, int start, int size,
                     int last, cst_audio_streaming_info *asi)
{
    if (start == 0) {
        i2s_set_sample_rates(I2S_NUM, w->sample_rate);  //ad = audio_open(w->sample_rate,w->num_channels,CST_AUDIO_LINEAR16);
    }

    size_t bytes_written = 0;
    i2s_write(I2S_NUM, &(w->samples[start]), size*sizeof(uint16_t), &bytes_written, 100);
    
    // /* if you want to stop return CST_AUDIO_STREAM_STOP */
    return CST_AUDIO_STREAM_CONT;
}

/** Arguments used by 'join' function */
static struct {
    struct arg_str *tts_text;
    struct arg_end *end;
} join_args;

static int connect(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &join_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, join_args.end, argv[0]);
        return 1;
    }
    ESP_LOGI(__func__, "Text to convert '%s'",
             join_args.tts_text->sval[0]);

    flite_synth_text(join_args.tts_text->sval[0],v);    
    // flite_synth_text("This is the demo of speech synthesis.",v);    
    return 0;
}

void register_tts(void)
{
    join_args.tts_text = arg_str1(NULL, NULL, "<tts_text>", "Text to covert to speech");
    join_args.end = arg_end(2);

    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .dma_desc_num = 6,
        .dma_frame_num = 60,
        .use_apll = false,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1                                //Interrupt level 1
    };
    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO                                               //Not used
    };
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);

    flite_init();
    // cst_voice *v = register_cmu_us_kal(NULL);
    v = register_cmu_us_kal(NULL);
    
    cst_audio_streaming_info *asi =
        cst_alloc(struct cst_audio_streaming_info_struct, 1);
    asi->min_buffsize = 256;
    asi->asc = i2s_stream_chunk;
    asi->userdata = NULL;
    feat_set(v->features, "streaming_info", audio_streaming_info_val(asi));

    const esp_console_cmd_t join_cmd = {
        .command = "tts",
        .help = "Convert text to speech",
        .hint = NULL,
        .func = &connect,
        .argtable = &join_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&join_cmd) );
}
