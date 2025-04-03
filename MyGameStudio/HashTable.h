#pragma once
#include <cstdint>
#include <string>

namespace containers
{
	template<typename T>
	struct HashItem
	{
		T ItemData;
		std::string ItemKey;
	};

	template<typename T>
	class HashTable
	{
	private:
		static uint32_t MurmurHash3(const std::string& key, uint32_t const seed = 0)
		{
			const uint32_t c1 = 0xcc9e2d51;
			const uint32_t c2 = 0x1b873593;
			const uint32_t r1 = 15;
			const uint32_t r2 = 13;
			const uint32_t m = 5;
			const uint32_t n = 0xe6546b64;

			uint32_t hash = seed;
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key.data());
			int len = key.size();

			while (len--) {
				uint32_t k = *data++;
				k *= c1;
				k = (k << r1) | (k >> (32 - r1));
				k *= c2;
				hash ^= k;
				hash = (hash << r2) | (hash >> (32 - r2));
				hash = hash * m + n;
			}

			return hash;
		}

		uint32_t size_ = 1024;
		HashItem<T>* itemList_ = new HashItem<T>[size_];

	public:
		HashTable() = default;
		explicit HashTable(uint32_t size): size_(size) { }
		~HashTable() = default;

		uint32_t Insert(const std::string& key, T value)
		{
			uint32_t itemHash = MurmurHash3(key) % size_;
			uint32_t itemIndex = itemHash;
			uint32_t k = 0;
			bool kPositive = true;

			while (!(itemList_[itemIndex].ItemKey.empty() || itemList_[itemIndex].ItemKey == key))
			{
				if (kPositive)
					k++;

				itemIndex = kPositive ? itemHash + k * k : itemHash - k * k;
				kPositive = !kPositive;

				if (itemIndex < 0 || itemIndex >= size_)
					return 0;
			}

			itemList_[itemIndex] = HashItem<T>{ value, key };

			return 1;
		}

		T Search(std::string key)
		{
			uint32_t itemHash = MurmurHash3(key) % size_;
			uint32_t itemIndex = itemHash;
			uint32_t k = 0;
			bool kPositive = true;

			while (itemList_[itemIndex].ItemKey != key)
			{
				if (kPositive)
					k++;

				itemIndex = kPositive ? itemHash + k * k : itemHash - k * k;
				kPositive = !kPositive;

				if (itemIndex < 0 || itemIndex >= size_)
					return nullptr;
			}

			return itemList_[itemIndex].ItemData;
		}

		uint32_t Delete(std::string key)
		{
			uint32_t itemHash = MurmurHash3(key) % size_;
			uint32_t itemIndex = itemHash;
			uint32_t k = 0;
			bool kPositive = true;

			while (itemList_[itemIndex].ItemKey != key)
			{
				if (kPositive)
					k++;

				itemIndex = kPositive ? itemHash + k * k : itemHash - k * k;
				kPositive = !kPositive;

				if (itemIndex < 0 || itemIndex >= size_)
					return 0;
			}

			itemList_[itemIndex].ItemKey = "";

			return 1;
		}

		class Proxy
		{
		private:
			HashTable& hashTable_;
			std::string index_;

		public:
			Proxy(HashTable& hashTable, std::string index): hashTable_(hashTable), index_(index) { }

			operator std::string() const
			{
				return hashTable_.Search(index_);
			}

			Proxy& operator= (T insertItem)
			{
				hashTable_.Insert(index_, insertItem);
				return *this;
			}
		};

		Proxy operator[] (const std::string& key)
		{
			return Proxy(*this, key);
		}
	};
}
