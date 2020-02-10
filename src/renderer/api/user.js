import axios from '@/libs/api.request'

export const login = ({ userName, password }) => {
  const data = {
    userName,
    password
  }
  return axios.request({
    url: 'login',
    data,
    method: 'post'
  })
}

export const getUserInfo = (token) => {
  return axios.request({
    url: 'get_info',
    params: {
      token
    },
    method: 'get'
  })
}

export const logout = (token) => {
  return axios.request({
    url: 'logout',
    method: 'post'
  })
}

export const getUnreadCount = () => {
  return axios.request({
    url: 'message/count',
    method: 'get'
  })
}

export const getMessage = () => {
  return axios.request({
    url: 'message/init',
    method: 'get'
  })
}

export const getContentByMsgId = msgid => {
  return axios.request({
    url: 'message/content',
    method: 'get',
    params: {
      msgid
    }
  })
}

export const hasRead = msgid => {
  return axios.request({
    url: 'message/has_read',
    method: 'post',
    data: {
      msgid
    }
  })
}

export const removeReaded = msgid => {
  return axios.request({
    url: 'message/remove_readed',
    method: 'post',
    data: {
      msgid
    }
  })
}

export const restoreTrash = msgid => {
  return axios.request({
    url: 'message/restore',
    method: 'post',
    data: {
      msgid
    }
  })
}
