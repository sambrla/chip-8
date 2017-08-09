#ifndef CHIP8_INTERPRETER_H_
#define CHIP8_INTERPRETER_H_

#include <string>

#define MEMORY_SIZE 4096
#define PROGRAM_START_ADDR 512 // Most progs start at 0x200 (512)

class Interpreter
{
  public:
    typedef unsigned char   u8;
    typedef unsigned short u16;

    struct FrameBuffer
    {
        static constexpr unsigned kWidth  = 64;
        static constexpr unsigned kHeight = 32;
        u8 pixels[kWidth * kHeight]{};
    };    
                                                                               
    Interpreter();

    // Execute a single 'cpu' cycle
    void cycle();

    // Load rom into interpreter memory
    void load_rom(std::string rom_path);

    // Hex keypad input handling. Keycode must be between 0x0 and 0xF (inc).
    void press_key(u8 keycode);
    void release_key(u8 keycode);

    // Current buzzer state
    // TODO: Implement
    bool buzzer_active() const;

    // Current framebuffer content
    const FrameBuffer* frame() const;

    // Debugging
    void dump_registers() const;
    void dump_memory(unsigned bytes, unsigned offset = 0) const;

  private:
    enum class OpCode
    {
        SYS_NNN,
        CLS,
        RET,
        JP_NNN,
        CALL_NNN,
        SE_Vx_KK,
        SNE_Vx_KK,
        SE_Vx_Vy,
        LD_Vx_KK,
        ADD_Vx_KK,
        LD_Vx_Vy,
        OR_Vx_Vy,
        AND_Vx_Vy,
        XOR_Vx_Vy,
        ADD_Vx_Vy,
        SUB_Vx_Vy,
        SHR_Vx,
        SUBN_Vx_Vy,
        SHL_Vx,
        SNE_Vx_Vy,
        LD_I_NNN,
        JP_V0_NNN,
        RND_Vx_KK,
        DRW_Vx_Vy_N,
        SKP_Vx,
        SKNP_Vx,
        LD_Vx_DT,
        LD_Vx_K,
        LD_DT_Vx,
        LD_ST_Vx,
        ADD_I_Vx,
        LD_F_Vx,
        LD_B_Vx,
        LD_I_Vx,
        LD_Vx_I,
        NOOP
    };

     u8 mem[MEMORY_SIZE]{/* value init */};
     u8 registers_v[16]{};
    u16 registers_i = 0;
     u8 registers_sound_timer = 0;
     u8 registers_delay_timer = 0;
    u16 stack[16]{};
     u8 stack_pointer = 0;
    u16 program_counter = PROGRAM_START_ADDR;

    FrameBuffer framebuffer;
    bool key_state[16]{};

    OpCode opcode(u16 instruction) const;
    void execute_instruction(u16 instruction);
    void load_font_sprites();
};

#endif // CHIP8_INTERPRETER_H_