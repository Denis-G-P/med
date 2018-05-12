// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: med.proto

#include "med.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
class plainDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<plain>
      _instance;
} _plain_default_instance_;
namespace protobuf_med_2eproto {
void InitDefaultsplainImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::_plain_default_instance_;
    new (ptr) ::plain();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::plain::InitAsDefaultInstance();
}

void InitDefaultsplain() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsplainImpl);
}

::google::protobuf::Metadata file_level_metadata[1];
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, int_32_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, int_64_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, uint_32_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, uint_64_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, sint_32_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, sint_64_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, bool_1_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, enum_1_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, fix_64_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, sfix_64_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, dreal_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, fix_32_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, sfix_32_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::plain, real_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::plain)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::_plain_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "med.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\tmed.proto\"\233\002\n\005plain\022\016\n\006int_32\030\001 \001(\005\022\016\n"
      "\006int_64\030\002 \001(\003\022\017\n\007uint_32\030\003 \001(\r\022\017\n\007uint_6"
      "4\030\004 \001(\004\022\017\n\007sint_32\030\005 \001(\021\022\017\n\007sint_64\030\006 \001("
      "\022\022\016\n\006bool_1\030\007 \001(\010\022\033\n\006enum_1\030\010 \001(\0162\013.plai"
      "n.Enum\022\016\n\006fix_64\030\n \001(\006\022\017\n\007sfix_64\030\013 \001(\020\022"
      "\r\n\005dreal\030\014 \001(\001\022\016\n\006fix_32\0302 \001(\007\022\017\n\007sfix_3"
      "2\0303 \001(\017\022\014\n\004real\0304 \001(\002\"\"\n\004Enum\022\010\n\004ZERO\020\000\022"
      "\007\n\003ONE\020\001\022\007\n\003TWO\020\002b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 305);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "med.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_med_2eproto
const ::google::protobuf::EnumDescriptor* plain_Enum_descriptor() {
  protobuf_med_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_med_2eproto::file_level_enum_descriptors[0];
}
bool plain_Enum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const plain_Enum plain::ZERO;
const plain_Enum plain::ONE;
const plain_Enum plain::TWO;
const plain_Enum plain::Enum_MIN;
const plain_Enum plain::Enum_MAX;
const int plain::Enum_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void plain::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int plain::kInt32FieldNumber;
const int plain::kInt64FieldNumber;
const int plain::kUint32FieldNumber;
const int plain::kUint64FieldNumber;
const int plain::kSint32FieldNumber;
const int plain::kSint64FieldNumber;
const int plain::kBool1FieldNumber;
const int plain::kEnum1FieldNumber;
const int plain::kFix64FieldNumber;
const int plain::kSfix64FieldNumber;
const int plain::kDrealFieldNumber;
const int plain::kFix32FieldNumber;
const int plain::kSfix32FieldNumber;
const int plain::kRealFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

plain::plain()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_med_2eproto::InitDefaultsplain();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:plain)
}
plain::plain(const plain& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&int_64_, &from.int_64_,
    static_cast<size_t>(reinterpret_cast<char*>(&real_) -
    reinterpret_cast<char*>(&int_64_)) + sizeof(real_));
  // @@protoc_insertion_point(copy_constructor:plain)
}

void plain::SharedCtor() {
  ::memset(&int_64_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&real_) -
      reinterpret_cast<char*>(&int_64_)) + sizeof(real_));
  _cached_size_ = 0;
}

plain::~plain() {
  // @@protoc_insertion_point(destructor:plain)
  SharedDtor();
}

void plain::SharedDtor() {
}

void plain::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* plain::descriptor() {
  ::protobuf_med_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_med_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const plain& plain::default_instance() {
  ::protobuf_med_2eproto::InitDefaultsplain();
  return *internal_default_instance();
}

plain* plain::New(::google::protobuf::Arena* arena) const {
  plain* n = new plain;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void plain::Clear() {
// @@protoc_insertion_point(message_clear_start:plain)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&int_64_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&real_) -
      reinterpret_cast<char*>(&int_64_)) + sizeof(real_));
  _internal_metadata_.Clear();
}

bool plain::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:plain)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(16383u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 int_32 = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &int_32_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int64 int_64 = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &int_64_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 uint_32 = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &uint_32_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint64 uint_64 = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(32u /* 32 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &uint_64_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // sint32 sint_32 = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 40 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_SINT32>(
                 input, &sint_32_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // sint64 sint_64 = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(48u /* 48 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_SINT64>(
                 input, &sint_64_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool bool_1 = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(56u /* 56 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &bool_1_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .plain.Enum enum_1 = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(64u /* 64 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_enum_1(static_cast< ::plain_Enum >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // fixed64 fix_64 = 10;
      case 10: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(81u /* 81 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED64>(
                 input, &fix_64_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // sfixed64 sfix_64 = 11;
      case 11: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(89u /* 89 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_SFIXED64>(
                 input, &sfix_64_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double dreal = 12;
      case 12: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(97u /* 97 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &dreal_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // fixed32 fix_32 = 50;
      case 50: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(149u /* 405 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &fix_32_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // sfixed32 sfix_32 = 51;
      case 51: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(157u /* 413 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_SFIXED32>(
                 input, &sfix_32_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float real = 52;
      case 52: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(165u /* 421 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &real_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:plain)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:plain)
  return false;
#undef DO_
}

void plain::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:plain)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 int_32 = 1;
  if (this->int_32() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->int_32(), output);
  }

  // int64 int_64 = 2;
  if (this->int_64() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->int_64(), output);
  }

  // uint32 uint_32 = 3;
  if (this->uint_32() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->uint_32(), output);
  }

  // uint64 uint_64 = 4;
  if (this->uint_64() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(4, this->uint_64(), output);
  }

  // sint32 sint_32 = 5;
  if (this->sint_32() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteSInt32(5, this->sint_32(), output);
  }

  // sint64 sint_64 = 6;
  if (this->sint_64() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteSInt64(6, this->sint_64(), output);
  }

  // bool bool_1 = 7;
  if (this->bool_1() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(7, this->bool_1(), output);
  }

  // .plain.Enum enum_1 = 8;
  if (this->enum_1() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      8, this->enum_1(), output);
  }

  // fixed64 fix_64 = 10;
  if (this->fix_64() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed64(10, this->fix_64(), output);
  }

  // sfixed64 sfix_64 = 11;
  if (this->sfix_64() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteSFixed64(11, this->sfix_64(), output);
  }

  // double dreal = 12;
  if (this->dreal() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(12, this->dreal(), output);
  }

  // fixed32 fix_32 = 50;
  if (this->fix_32() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(50, this->fix_32(), output);
  }

  // sfixed32 sfix_32 = 51;
  if (this->sfix_32() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteSFixed32(51, this->sfix_32(), output);
  }

  // float real = 52;
  if (this->real() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(52, this->real(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:plain)
}

::google::protobuf::uint8* plain::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:plain)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 int_32 = 1;
  if (this->int_32() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->int_32(), target);
  }

  // int64 int_64 = 2;
  if (this->int_64() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->int_64(), target);
  }

  // uint32 uint_32 = 3;
  if (this->uint_32() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->uint_32(), target);
  }

  // uint64 uint_64 = 4;
  if (this->uint_64() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(4, this->uint_64(), target);
  }

  // sint32 sint_32 = 5;
  if (this->sint_32() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteSInt32ToArray(5, this->sint_32(), target);
  }

  // sint64 sint_64 = 6;
  if (this->sint_64() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteSInt64ToArray(6, this->sint_64(), target);
  }

  // bool bool_1 = 7;
  if (this->bool_1() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(7, this->bool_1(), target);
  }

  // .plain.Enum enum_1 = 8;
  if (this->enum_1() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      8, this->enum_1(), target);
  }

  // fixed64 fix_64 = 10;
  if (this->fix_64() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed64ToArray(10, this->fix_64(), target);
  }

  // sfixed64 sfix_64 = 11;
  if (this->sfix_64() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteSFixed64ToArray(11, this->sfix_64(), target);
  }

  // double dreal = 12;
  if (this->dreal() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(12, this->dreal(), target);
  }

  // fixed32 fix_32 = 50;
  if (this->fix_32() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(50, this->fix_32(), target);
  }

  // sfixed32 sfix_32 = 51;
  if (this->sfix_32() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteSFixed32ToArray(51, this->sfix_32(), target);
  }

  // float real = 52;
  if (this->real() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(52, this->real(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:plain)
  return target;
}

size_t plain::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:plain)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // int64 int_64 = 2;
  if (this->int_64() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->int_64());
  }

  // int32 int_32 = 1;
  if (this->int_32() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->int_32());
  }

  // uint32 uint_32 = 3;
  if (this->uint_32() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->uint_32());
  }

  // uint64 uint_64 = 4;
  if (this->uint_64() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->uint_64());
  }

  // sint64 sint_64 = 6;
  if (this->sint_64() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::SInt64Size(
        this->sint_64());
  }

  // sint32 sint_32 = 5;
  if (this->sint_32() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::SInt32Size(
        this->sint_32());
  }

  // bool bool_1 = 7;
  if (this->bool_1() != 0) {
    total_size += 1 + 1;
  }

  // fixed64 fix_64 = 10;
  if (this->fix_64() != 0) {
    total_size += 1 + 8;
  }

  // sfixed64 sfix_64 = 11;
  if (this->sfix_64() != 0) {
    total_size += 1 + 8;
  }

  // double dreal = 12;
  if (this->dreal() != 0) {
    total_size += 1 + 8;
  }

  // .plain.Enum enum_1 = 8;
  if (this->enum_1() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->enum_1());
  }

  // fixed32 fix_32 = 50;
  if (this->fix_32() != 0) {
    total_size += 2 + 4;
  }

  // sfixed32 sfix_32 = 51;
  if (this->sfix_32() != 0) {
    total_size += 2 + 4;
  }

  // float real = 52;
  if (this->real() != 0) {
    total_size += 2 + 4;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void plain::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:plain)
  GOOGLE_DCHECK_NE(&from, this);
  const plain* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const plain>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:plain)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:plain)
    MergeFrom(*source);
  }
}

void plain::MergeFrom(const plain& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:plain)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.int_64() != 0) {
    set_int_64(from.int_64());
  }
  if (from.int_32() != 0) {
    set_int_32(from.int_32());
  }
  if (from.uint_32() != 0) {
    set_uint_32(from.uint_32());
  }
  if (from.uint_64() != 0) {
    set_uint_64(from.uint_64());
  }
  if (from.sint_64() != 0) {
    set_sint_64(from.sint_64());
  }
  if (from.sint_32() != 0) {
    set_sint_32(from.sint_32());
  }
  if (from.bool_1() != 0) {
    set_bool_1(from.bool_1());
  }
  if (from.fix_64() != 0) {
    set_fix_64(from.fix_64());
  }
  if (from.sfix_64() != 0) {
    set_sfix_64(from.sfix_64());
  }
  if (from.dreal() != 0) {
    set_dreal(from.dreal());
  }
  if (from.enum_1() != 0) {
    set_enum_1(from.enum_1());
  }
  if (from.fix_32() != 0) {
    set_fix_32(from.fix_32());
  }
  if (from.sfix_32() != 0) {
    set_sfix_32(from.sfix_32());
  }
  if (from.real() != 0) {
    set_real(from.real());
  }
}

void plain::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:plain)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void plain::CopyFrom(const plain& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:plain)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool plain::IsInitialized() const {
  return true;
}

void plain::Swap(plain* other) {
  if (other == this) return;
  InternalSwap(other);
}
void plain::InternalSwap(plain* other) {
  using std::swap;
  swap(int_64_, other->int_64_);
  swap(int_32_, other->int_32_);
  swap(uint_32_, other->uint_32_);
  swap(uint_64_, other->uint_64_);
  swap(sint_64_, other->sint_64_);
  swap(sint_32_, other->sint_32_);
  swap(bool_1_, other->bool_1_);
  swap(fix_64_, other->fix_64_);
  swap(sfix_64_, other->sfix_64_);
  swap(dreal_, other->dreal_);
  swap(enum_1_, other->enum_1_);
  swap(fix_32_, other->fix_32_);
  swap(sfix_32_, other->sfix_32_);
  swap(real_, other->real_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata plain::GetMetadata() const {
  protobuf_med_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_med_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
