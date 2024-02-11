#ifndef _PROTOBUF_CALLBACKS_
#define _PROTOBUF_CALLBACKS_

#include "hydroponic_data.pb.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

bool write_string(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool msg_callback(pb_istream_t *stream, const pb_field_t *field, void **arg);

#endif