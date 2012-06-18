#include <node.h>

namespace defer_tick {

using namespace node;
using namespace v8;

#ifndef offset_of
// g++ in strict mode complains loudly about the system offsetof() macro
// because it uses NULL as the base address.
# define offset_of(type, member) \
  ((intptr_t) ((char *) &(((type *) 8)->member) - 8))
#endif

#ifndef container_of
# define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offset_of(type, member)))
#endif

struct DeferRequest {
  Persistent<Function> callback;
  uv_work_t work;
};


static void DeferNop(uv_work_t* work) {
}


static void DeferCallback(uv_work_t* work) {
  HandleScope scope;

  DeferRequest* req = container_of(work, DeferRequest, work);
  assert(!req->callback.IsEmpty());
  req->callback->Call(Context::GetCurrent()->Global(), 0, NULL);

  req->callback.Dispose();
  req->callback.Clear();

  delete req;
}


static Handle<Value> DeferTick(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1 || !args[0]->IsFunction()) {
    return ThrowException(String::New("First argument must be function"));
  }

  DeferRequest* req = new DeferRequest();
  req->callback = Persistent<Function>::New(args[0].As<Function>());
  uv_queue_work(uv_default_loop(), &req->work, DeferNop, DeferCallback);

  return Null();
}


static void Init(Handle<Object> target) {
  NODE_SET_METHOD(target, "deferTick", DeferTick);
}


NODE_MODULE(defer_tick, Init);

} // namespace defer_tick
