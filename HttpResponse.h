#ifndef HttpResponse_h
#define HttpResponse_h

class HttpResponse {
public:
  int StatusCode = -1;
  String Body = "";

  HttpResponse(int statusCode){
    StatusCode = statusCode;
  }

  bool IsSuccess() {
    return StatusCode >= 200 && StatusCode <= 399;
  }
};

#endif