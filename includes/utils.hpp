#ifndef UTILS_HPP
#define UTILS_HPP

// source: https://stackoverflow.com/questions/44517556
const struct s_nullptr {

    public:

        /* Constructors and Destructors */
        template< class T >
        operator T*(void) const {
            return (0);
        };

        template< class C, class T >
        operator T C ::*(void) const {
            return (0);
        };

    private:

        /* Private Attributes */
        void operator&(void) const;

} nullptr_t = {};

#endif /* UTILS_HPP */
