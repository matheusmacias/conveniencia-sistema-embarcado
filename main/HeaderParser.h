class HeaderParser {
  private:
    String headers;
  public:
    HeaderParser(String headers) {
      this->headers = headers;
    }

    String getHeader(String name) {
      int index = headers.indexOf(name);
      if (index != -1) {
        int startIndex = index + name.length() + 2; // considera ':' e espaço em branco
        int endIndex = headers.indexOf("\n", startIndex);
        if (endIndex != -1) {
          return headers.substring(startIndex, endIndex);
        }
      }
      return "";
    }

    String getHost() {
      return getHeader("Host");
    }

    String getUserAgent() {
      return getHeader("User-Agent");
    }

    String getAuthorization(bool onlyToken = false) {
      String authorizationHeader = getHeader("Authorization");
      if (authorizationHeader != "") {
        if (onlyToken) {
          return authorizationHeader.substring(7); // pula os primeiros 7 caracteres ("Bearer ")
        } else {
          return authorizationHeader;
        }
      }
      return "";
    }

    String getContentType() {
      return getHeader("Content-Type");
    }

    String getAccept() {
      return getHeader("Accept");
    }

    int getContentLength() {
      String contentLengthHeader = getHeader("Content-Length");
      if (contentLengthHeader != "") {
        return contentLengthHeader.toInt();
      }
      return 0;
    }
};
