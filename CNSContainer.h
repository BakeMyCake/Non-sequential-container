class NSC_OOR_ERR : public std::exception
{
	virtual const char* what() const throw()
	{
		 return "NSContainer access out of range";
	}
};

class NSC_BA_ERR : public std::exception
{
	virtual const char* what() const throw()
	{
		return "NSContainer out of memory";
	}
};

template<typename A_Type>
class CNSContainer
{
	NSC_OOR_ERR NSC_OOR_m;
	NSC_BA_ERR NSC_BA_m;
	std::vector<std::shared_ptr<A_Type>> pointers_m;
public:
	size_t size()
	{
		return pointers_m.size();
	}

	template <class... Args>
	std::weak_ptr<A_Type> push(Args&&... args)
	{
		try {
			std::shared_ptr<A_Type> ptr(new A_Type(std::forward<Args>(args)...));
		}
		catch (std::bad_alloc& ba) {
			throw NSC_BA_m;
		}
		pointers_m.emplace_back(ptr);
		return ptr;
	}

	void remove(const size_t& key)
	{
		pointers_m.erase(pointers_m.begin() + key);
	}

	std::weak_ptr<A_Type> operator[](const size_t& key)
	{
		try {
			return pointers_m[key];
		}
		catch (const std::out_of_range& oor) {
			throw NSC_OOR_m;
		}
	}
};
