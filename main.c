#include "quickjs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义一个函数，用来实现 console.log
static JSValue js_console_log(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv) {
    for (int i = 0; i < argc; i++) {
        const char *str = JS_ToCString(ctx, argv[i]);
        if (str) {
            printf("%s ", str);
            JS_FreeCString(ctx, str);
        }
    }
    printf("\n");
    return JS_UNDEFINED;
}

// 注册 console 对象和 log 方法
static void js_std_add_console(JSContext *ctx) {
    JSValue global_obj, console;

    // 获取全局对象
    global_obj = JS_GetGlobalObject(ctx);

    // 创建 console 对象R
    console = JS_NewObject(ctx);

    // 给 console 对象添加 log 方法
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_console_log, "log", 1));

    // 将 console 对象绑定到全局对象中
    JS_SetPropertyStr(ctx, global_obj, "console", console);

    JS_FreeValue(ctx, global_obj);
}


/**
 * http://numcalc.com/
 */
int main() {
    JSRuntime *rt;
    JSContext *ctx;
    const char *js_code = "(function() { return 52; })()";

    // 初始化 QuickJS 运行时和上下文
    rt = JS_NewRuntime();
    if (!rt) {
        printf("Failed to initialize QuickJS runtime.\n");
        return 1;
    }

    ctx = JS_NewContext(rt);
    if (!ctx) {
        printf("Failed to initialize QuickJS context.\n");
        JS_FreeRuntime(rt);
        return 1;
    }

    // 注册 console.log 函数
    js_std_add_console(ctx);

    // 执行 JavaScript 代码
    // JS_Eval(ctx, js_code, strlen(js_code), "index.js", JS_EVAL_TYPE_GLOBAL);
    const JSValue val = JS_Eval(ctx, js_code, strlen(js_code), "index.js", JS_EVAL_TYPE_GLOBAL);
    // const JSValue val = JS_Eval(ctx, js_code, strlen(js_code), "index.js", JS_EVAL_TYPE_MODULE);

    printf("val.tag %ld\n", val.tag);

    // 检查返回值是否是错误
    if (JS_IsException(val)) {
        printf("Error executing JS code.\n");
        JSValue exception = JS_GetException(ctx);
        const char *error = JS_ToCString(ctx, exception);
        printf("JS Exception: %s\n", error);
        JS_FreeCString(ctx, error);
        JS_FreeValue(ctx, exception);
    } else {
        // 输出结果
        int result;
        if (JS_ToInt32(ctx, &result, val) == 0) {
            printf("Returned value: %d\n", result);
        }
    }

    // 释放 JSValue
    JS_FreeValue(ctx, val);

    // 释放资源®
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return 0;
}
