#pragma once
#include <string>
#include <memory>

#include "IResource.h"

namespace ngenius
{
	class ILoader
	{
	public:
		ILoader(const ILoader&) = delete;
		ILoader(ILoader&&) noexcept = delete;
		ILoader& operator=(const ILoader&) = delete;
		ILoader& operator=(ILoader&&) noexcept = delete;
		virtual ~ILoader() = default;

		virtual const type_info& GetType() const = 0;

	protected:
		explicit ILoader() = default;
	};

	template<typename CONTENT_TYPE, typename... ARG_TYPE>
	class IBaseLoader : public ILoader
	{
	public:
		explicit IBaseLoader() = default;
		IBaseLoader(const IBaseLoader&) = delete;
		IBaseLoader(IBaseLoader&&) noexcept = delete;
		IBaseLoader& operator=(const IBaseLoader&) = delete;
		IBaseLoader& operator=(IBaseLoader&&) noexcept = delete;
		virtual ~IBaseLoader() = default;

		virtual const type_info& GetType() const { return typeid(CONTENT_TYPE); }

		virtual std::string GenerateResourceID(const std::string& path, ARG_TYPE&&...) const { return path; }
		virtual std::shared_ptr<CONTENT_TYPE> LoadContent(const std::string& path, ARG_TYPE&&...) const = 0;
	};
}

