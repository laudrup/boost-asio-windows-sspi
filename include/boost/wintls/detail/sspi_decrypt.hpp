//
// Copyright (c) 2021 Kasper Laudrup (laudrup at stacktrace dot dk)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_WINTLS_DETAIL_SSPI_DECRYPT_HPP
#define BOOST_WINTLS_DETAIL_SSPI_DECRYPT_HPP

#include <boost/wintls/detail/sspi_functions.hpp>
#include <boost/wintls/detail/decrypt_buffers.hpp>

#include <boost/assert.hpp>

#include <vector>

namespace boost {
namespace wintls {
namespace detail {

class sspi_decrypt {
public:
  enum class state {
    data_needed,
    data_available,
    error
  };

  sspi_decrypt(CtxtHandle* context)
    : size_decrypted(0)
    , input_buffer(net::buffer(encrypted_data_))
    , context_(context)
    , last_error_(SEC_E_OK) {
    buffers_[0].pvBuffer = encrypted_data_.data();
  }

  template <class MutableBufferSequence>
  state operator()(const MutableBufferSequence& output_buffers) {
    if (!decrypted_data_.empty()) {
      size_decrypted = net::buffer_copy(output_buffers, net::buffer(decrypted_data_));
      decrypted_data_.erase(decrypted_data_.begin(), decrypted_data_.begin() + size_decrypted);
      return state::data_available;
    }

    if (buffers_[0].cbBuffer == 0) {
      input_buffer = net::buffer(encrypted_data_);
      return state::data_needed;
    }

    buffers_[0].BufferType = SECBUFFER_DATA;
    buffers_[1].BufferType = SECBUFFER_EMPTY;
    buffers_[2].BufferType = SECBUFFER_EMPTY;
    buffers_[3].BufferType = SECBUFFER_EMPTY;

    input_buffer = net::buffer(encrypted_data_) + buffers_[0].cbBuffer;
    const auto size = buffers_[0].cbBuffer;
    last_error_ = detail::sspi_functions::DecryptMessage(context_, buffers_, 0, nullptr);

    if (last_error_ == SEC_E_INCOMPLETE_MESSAGE) {
      buffers_[0].cbBuffer = size;
      return state::data_needed;
    }

    if (last_error_ != SEC_E_OK) {
      return state::error;
    }

    if (buffers_[1].BufferType == SECBUFFER_DATA) {
      const auto data_ptr = reinterpret_cast<const char*>(buffers_[1].pvBuffer);
      const auto data_size = buffers_[1].cbBuffer;
      size_decrypted = net::buffer_copy(output_buffers, net::buffer(data_ptr, data_size));
      if (size_decrypted < data_size) {
        std::copy(data_ptr + size_decrypted, data_ptr + data_size, std::back_inserter(decrypted_data_));
      }
    }

    if (buffers_[3].BufferType == SECBUFFER_EXTRA) {
      const auto extra_size = buffers_[3].cbBuffer;
      std::memmove(encrypted_data_.data(), buffers_[3].pvBuffer, extra_size);
      buffers_[0].cbBuffer = extra_size;
    } else {
      buffers_[0].cbBuffer = 0;
    }

    return state::data_available;
  }

  void size_read(std::size_t size) {
    buffers_[0].cbBuffer += static_cast<unsigned long>(size);
    input_buffer = net::buffer(encrypted_data_) + buffers_[0].cbBuffer;
  }

  std::size_t size_decrypted;
  net::mutable_buffer input_buffer;

  boost::system::error_code last_error() const {
    return error::make_error_code(last_error_);
  }

private:
  CtxtHandle* context_;
  SECURITY_STATUS last_error_;
  decrypt_buffers buffers_;
  std::array<char, 0x10000> encrypted_data_;
  std::vector<char> decrypted_data_;
};

} // namespace detail
} // namespace wintls
} // namespace boost

#endif // BOOST_WINTLS_DETAIL_SSPI_DECRYPT_HPP
