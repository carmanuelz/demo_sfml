#ifndef USERDATA_H
#define USERDATA_H


namespace sse
{
    class UserData
    {
        public:
            int tipo;
            int estado;
            UserData();
            virtual ~UserData();
            int Gettipo() { return tipo; }
            void Settipo(int val) { tipo = val; }
            int Getestado() { return estado; }
            void Setestado(int val) { estado = val; }
        protected:
        private:
    };
}

#endif // USERDATA_H
