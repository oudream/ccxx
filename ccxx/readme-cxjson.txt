// todo:
// Value：：IsObject 中缺少模糊查找功能：如：/*/name == "name1"
// Value：：IsArray 中缺少插入
// Document：：缺少总点空间大小函数


/*
 *
 * 中文帮助： http://rapidjson.org/zh-cn/ ，简单的加载与保存请看 CxJson::load 与 CxJson::save
 ***注意1：
 ***注意1：
 ***注意1：
    Document Value 的操作必须知道其类型，不然会出 assert，
     ***以下是 Value IsObject 方面的 例如 ：
        SizeType MemberCount() const { RAPIDJSON_ASSERT(IsObject()); return data_.o.size; }
        bool ObjectEmpty() const { RAPIDJSON_ASSERT(IsObject()); return data_.o.size == 0; }
        ConstMemberIterator MemberBegin() const { RAPIDJSON_ASSERT(IsObject()); return ConstMemberIterator(data_.o.members); }
        ConstMemberIterator MemberEnd() const   { RAPIDJSON_ASSERT(IsObject()); return ConstMemberIterator(data_.o.members + data_.o.size); }
        MemberIterator FindMember(const GenericValue<Encoding, SourceAllocator>& name) { RAPIDJSON_ASSERT(IsObject()); ...}
        GenericValue& AddMember(GenericValue& name, GenericValue& value, Allocator& allocator) { RAPIDJSON_ASSERT(IsObject()); ...}
        void RemoveAllMembers() { RAPIDJSON_ASSERT(IsObject()); ...}
        MemberIterator EraseMember(ConstMemberIterator first, ConstMemberIterator last) { RAPIDJSON_ASSERT(IsObject()); ...}
     ***以下是 Value IsArray 方面的 例如 ：
        SizeType Size() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.size; }
        SizeType Capacity() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.capacity; }
        bool Empty() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.size == 0; }
        void Clear() { RAPIDJSON_ASSERT(IsArray()); ...}
        GenericValue& operator[](SizeType index) { RAPIDJSON_ASSERT(IsArray()); ...}
        ValueIterator Begin() { RAPIDJSON_ASSERT(IsArray()); return data_.a.elements; }
        ValueIterator End() { RAPIDJSON_ASSERT(IsArray()); return data_.a.elements + data_.a.size; }
         GenericValue& Reserve(SizeType newCapacity, Allocator &allocator) { RAPIDJSON_ASSERT(IsArray()); ...}
        GenericValue& PushBack(GenericValue& value, Allocator& allocator) { RAPIDJSON_ASSERT(IsArray()); ...}
        GenericValue& PopBack() { RAPIDJSON_ASSERT(IsArray()); ...}
        ValueIterator Erase(ConstValueIterator first, ConstValueIterator last) { RAPIDJSON_ASSERT(IsArray()); ...}
     ***以下是 Value GetXXX 方面的 例如 ：
        int GetInt() const          { RAPIDJSON_ASSERT(flags_ & kIntFlag);   return data_.n.i.i;   }
        unsigned GetUint() const    { RAPIDJSON_ASSERT(flags_ & kUintFlag);  return data_.n.u.u;   }
        int64_t GetInt64() const    { RAPIDJSON_ASSERT(flags_ & kInt64Flag); return data_.n.i64; }
        uint64_t GetUint64() const  { RAPIDJSON_ASSERT(flags_ & kUint64Flag); return data_.n.u64; }
        double GetDouble() const { RAPIDJSON_ASSERT(IsNumber()); ... }
        const Ch* GetString() const { RAPIDJSON_ASSERT(IsString()); return ((flags_ & kInlineStrFlag) ? data_.ss.str : data_.s.str); }
        SizeType GetStringLength() const { RAPIDJSON_ASSERT(IsString()); return ((flags_ & kInlineStrFlag) ? (data_.ss.GetLength()) : data_.s.length); }

    Document.Parse 可能会 assert
 *
 *
 ***注意2：
 ***注意2：
 ***注意2：
 ***转移语义（Move Semantics） http://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html#MoveSemantics
    在设计 RapidJSON 时有一个非常特别的决定，就是 Value 赋值并不是把来源 Value 复制至目的 Value，而是把把来源 Value 转移（move）至目的 Value。例如：
    Value a(123);
    Value b(456);
    b = a;         // a 变成 Null，b 变成数字 123。
 ***注意： 其实包括 b.PushBack(a, d.GetAllocator()) 或  b.AddMember("key1", a, d.GetAllocator()) 等有 operator= 操作的都有的
 *
 *
 ***注意3：
 ***注意3：
 ***注意3：
 ***转移语义（Move Semantics） http://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html#MoveSemantics
    在设计 RapidJSON 时有一个非常特别的决定，就是 Value 赋值并不是把来源 Value 复制至目的 Value，而是把把来源 Value 转移（move）至目的 Value。例如：
    Value a(123);
    Value b(456);
    b = a;         // a 变成 Null，b 变成数字 123。
 ***注意： 其实包括 b.PushBack(a, d.GetAllocator()) 或  b.AddMember("key1", a, d.GetAllocator()) 等有 operator= 操作的都有的
 *
*/
