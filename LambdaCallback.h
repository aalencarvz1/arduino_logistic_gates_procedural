#ifndef LAMBDACALLBACK_H
#define LAMBDACALLBACK_H


class ICallback {
  public:
    virtual void call() = 0; // Função virtual pura
};

template<typename T>
class LambdaCallback : public ICallback {
  public: 
    T lambda;

    LambdaCallback(const T& lambda) : lambda(lambda) {}

    void call() override {
      lambda(); // Executa a lambda capturada
    }
};

#endif //LAMBDACALLBACK_H