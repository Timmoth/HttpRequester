#ifndef HttpResponse_h
#define HttpResponse_h

class HttpResponse {
public:
  int StatusCode = -1;
  String Body = "";

  bool IsSuccess() {
    return StatusCode >= 200 && StatusCode <= 299;
  }
};

#endif