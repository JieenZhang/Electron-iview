import axios from '@/libs/api.request'
const WfbsClient = require('@/libs/WFBSManagerLib.js').WFBSManagerLib
export const login = ({ userName, password }) => {
  // const data = {
  //   userName,
  //   password
  // }
  // return axios.request({
  //   url: 'login',
  //   data,
  //   method: 'post'
  // })
  let rt = new Promise((resolve, reject) => {
    let magLib = new WfbsClient()
    magLib.Initialize('192.168.10.5', 5526, function (result, data) {
      if (result) {
        console.log('初始化成功!')
        magLib.Login(userName, password, function (result2) {
          if (result2) {
            resolve({token: {
              name: 'super_admin',
              user_id: '1',
              access: ['super_admin', 'admin'],
              token: 'super_admin',
              avatar: 'https://file.iviewui.com/dist/a0e88e83800f138b94d2414621bd9704.png'
            }})
          } else {
            reject(new Error('用户名或密码错误，登录失败！'))
          }
        })
      } else {
        reject(new Error(data))
      }
    })
  })
  return rt
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
