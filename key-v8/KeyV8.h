#pragma once

#include <memory>

#include <key-v8/lib_key_v8.h>
#include <key-v8/expose_headers.hpp>
#include <key-v8/PersistentV8.h>

#include <cvv8/arguments.hpp>

namespace key
{
	class PersistentKeyV8;

	class KeyV8
	{
	private:
		v8::Persistent<v8::Context> context;
		std::string working_dir;
		void LoadEnvironment();
	public:
		KeyV8(std::string working_dir);
		~KeyV8();

		static std::shared_ptr<key::PersistentKeyV8> KeyV8::New(std::string working_dir);

		void Log( std::string val ) { std::cout << val << std::endl; }
		v8::Handle<v8::Value> ExecuteFile(std::string short_filename);
		static bool ExecuteScript(v8::Handle<v8::String> script, std::string & short_filename);

		template<class T>
		static void NewObjectRef(key::PersistentV8<T> & persistentRef, v8::Handle<v8::Context> context, int argc, v8::Handle<v8::Value> * argv)
		{
			v8::HandleScope handle_scope;

			auto & cc = KeyV8::Class<T>();
			auto value = cc.CtorFunction()->CallAsConstructor(argc, argv);

			persistentRef.Assign(value, true);
		}

		template<class T>
		static std::shared_ptr<key::PersistentV8<T>> NewSharedObject(v8::Handle<v8::Context> context, int argc, v8::Handle<v8::Value> * argv)
		{
			v8::HandleScope handle_scope;

			auto & cc = KeyV8::Class<T>();
			auto value = cc.CtorFunction()->CallAsConstructor(argc, argv);

			return std::make_shared<key::PersistentV8<T>>(value);
		}

		template<class T>
		static cvv8::ClassCreator<T> & Class() {
			cvv8::ClassCreator<T> & cc(cvv8::ClassCreator<T>::Instance());

			if( cc.IsSealed() )
				return cc;

			v8::Handle<v8::ObjectTemplate> const & proto( cc.Prototype() );
			std::vector<std::string> items;

			T::reflect(items, cc, proto, false);
			T::reflect(items, cc, proto, true);
			auto ctor = cc.CtorFunction();
			ctor->Set(JSTR("__fields"), cvv8::CastToJS(items));

			return cc;
		}

		template<class T>
		void Reflect() {
			v8::HandleScope handle_scope;
			v8::Context::Scope context_scope(context);

			auto cc = KeyV8::Class<T>();
			cc.AddClassTo( cvv8::TypeName<T>::Value, context->Global() );
		}

		typedef cvv8::Signature<key::KeyV8 (
			cvv8::CtorForwarder<key::KeyV8 *(std::string)>
		)> Ctors;
	};

	class PersistentKeyV8
		: public PersistentV8<KeyV8>
	{
	public:
		PersistentKeyV8(v8::Handle<v8::Value> handle) 
			: PersistentV8<KeyV8>(handle) {}

		template<class T>
		void Reflect() { this->native->Reflect<T>(); }
		void Run(std::string js_relative_file_path) 
		{ 
			this->native->ExecuteFile(js_relative_file_path);
		}
	};
}

namespace cvv8 {
    CVV8_TypeName_DECL((key::KeyV8));

    template <>
    class ClassCreator_Factory<key::KeyV8>
     : public ClassCreator_Factory_Dispatcher< key::KeyV8,
          CtorArityDispatcher<key::KeyV8::Ctors> >
    {};

    template <>
    struct JSToNative< key::KeyV8 > : JSToNative_ClassCreator< key::KeyV8 >
    {};
}