
#include <linux/kconfig.h>
#include <linux/bpf.h>

#include <uapi/linux/ptrace.h>

// definitions of bpf helper functions we need, as found in
// http://elixir.free-electrons.com/linux/latest/source/samples/bpf/bpf_helpers.h

#define SEC(NAME) __attribute__((section(NAME), used))

#define PT_REGS_PARM1(x) ((x)->di)

static int (*bpf_probe_read)(void *dst, int size, void *unsafe_ptr) =
        (void *) BPF_FUNC_probe_read;
static int (*bpf_trace_printk)(const char *fmt, int fmt_size, ...) =
        (void *) BPF_FUNC_trace_printk;

#define printt(fmt, ...)                                                   \
        ({                                                                 \
                char ____fmt[] = fmt;                                      \
                bpf_trace_printk(____fmt, sizeof(____fmt), ##__VA_ARGS__); \
        })

// the kprobe

SEC("kprobe/SyS_open")
int kprobe__sys_open(struct pt_regs *ctx)
{
        char filename[256];

        bpf_probe_read(filename, sizeof(filename), (void *)PT_REGS_PARM1(ctx));

        printt("open(%s)\n", filename);

        return 0;
}

char _license[] SEC("license") = "GPL";
// this number will be interpreted by the elf loader
// to set the current running kernel version
__u32 _version SEC("version") = 0xFFFFFFFE;