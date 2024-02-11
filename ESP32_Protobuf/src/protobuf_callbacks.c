#include "protobuf_callbacks.h"


bool write_string(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)*arg, strlen((char*)*arg));
}

bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint8_t buffer[128] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    /* Print the string, in format comparable with protoc --decode.
     * Format comes from the arg defined in main().
     */
    //printf((char*)*arg, buffer); 
    strcpy((char*)*arg, (char*)buffer);
    return true;
}

bool msg_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{

    // hydroponic_Hydroponic *topmsg = field->message;
    // ESP_LOGI(TAG,"Message Type: %d" , (int)topmsg->messageType);

    if (field->tag == hydroponic_Hydroponic_dataPackage_tag)
    {


        hydroponic_DataPackage *message = field->pData;

        message->sector.funcs.decode =& read_string;
        message->sector.arg = malloc(10*sizeof(char));

    }

    else if (field->tag == hydroponic_Hydroponic_messageOk_tag)
    {
        hydroponic_MessageOk *message = field->pData;

        message->responseMessage.funcs.decode =& read_string;
        message->responseMessage.arg = malloc(50*sizeof(char));
       
    }

    else if (field->tag == hydroponic_Hydroponic_messageError_tag)
    {

        hydroponic_MessageError *message = field->pData;

        message->errorType.funcs.decode =& read_string;
        message->errorType.arg = malloc(50*sizeof(char));

    }

    else if (field->tag == hydroponic_Hydroponic_messageTimeout_tag)
    {
        hydroponic_MessageTimeout *message = field->pData;

        message->timeoutMessage.funcs.decode =& read_string;
        message->timeoutMessage.arg = malloc(50*sizeof(char));
      
    }

    return true;
}