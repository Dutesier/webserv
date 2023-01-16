#ifndef HTTP_VALIDATOR_HPP
#define HTTP_VALIDATOR_HPP

#include "http/HTTPRequest.hpp"
#include "utils/smt.hpp"

// # include "HTTPResponse.cpp"

// In the future we can expand this design pattern to check the valiity
// of HTTP Responses as well. (Pointer Polymorphism)

namespace webserv {

class ok_err {
    public:

        bool ok = true;
        int  err = 0;
};

class IHTTPValidator {
    public:

        virtual ok_err isValid(smt::shared_ptr<HTTPRequest>& req) = 0;
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

        ok_err isValid(smt::shared_ptr<HTTPRequest>& req) {
            if (this->m_next) { return this->m_next->isValid(req); }
            return {};
        }

    private:

        smt::shared_ptr<IHTTPValidator> m_next;
};

class hasMethod : public AHTTPValidator {
    public:

        ok_err isValid(smt::shared_ptr<HTTPRequest>& req) {
            webserv::Method method = req->getMethod();
            ok_err          ret;

            if (method == webserv::UNDEFINED) {
                ret.ok = false;
                ret.err = 1;
                return ret;
            }
            return AHTTPValidator::isValid(req);
        }
};

} // namespace webserv

#endif
