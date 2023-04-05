#ifndef HTTP_VALIDATOR_HPP
#define HTTP_VALIDATOR_HPP

#include "http/HTTPRequest.hpp"
#include "utils/smt.hpp"

// # include "HTTPResponse.cpp"

// In the future we can expand this design pattern to check the valiity
// of HTTP Responses as well. (Pointer Polymorphism)

class okErr {
    public:

        okErr(void);
        ~okErr(void);
        bool ok;
        int  err;
};

class IHTTPValidator {
    public:

        virtual ~IHTTPValidator(void) {}

        virtual okErr isValid(smt::shared_ptr<webserv::HTTPRequest>& req) = 0;
        virtual smt::shared_ptr<IHTTPValidator>
            setNext(smt::shared_ptr<IHTTPValidator> validator) = 0;
};

class AHTTPValidator {
    public:

        AHTTPValidator() : m_next(NULL) {}

        smt::shared_ptr<IHTTPValidator>
            setNext(smt::shared_ptr<IHTTPValidator> validator) {
            this->m_next = validator;
            return validator; // So that we can link validators:
                              // setNext(hasMethod)->setNext(hasVersion) etc.
        }

        okErr isValid(smt::shared_ptr<webserv::HTTPRequest>& req) {
            if (this->m_next) { return this->m_next->isValid(req); }
            return okErr();
        }

    private:

        smt::shared_ptr<IHTTPValidator> m_next;
};

class hasMethod : public AHTTPValidator {
    public:

        okErr isValid(smt::shared_ptr<webserv::HTTPRequest>& req) {
            webserv::HTTPRequest::Method method = req->getMethod();
            okErr                       ret;

            if (method == webserv::HTTPRequest::UNDEFINED) {
                ret.ok = false;
                ret.err = 1;
                return ret;
            }
            return AHTTPValidator::isValid(req);
        }
};

#endif
