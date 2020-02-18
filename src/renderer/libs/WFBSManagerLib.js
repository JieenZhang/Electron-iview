(function () {
  'use strict'

  const util = require('util')
  const EventEmitter = require('events').EventEmitter
  const WFBSManagerWrap = require('./binding/Release/electron-v2.0-win32-x64/WFBSManagerNodeJS.node')

  /**
     * 客户端库构造函数
     * @constructor
     */
  function WFBSManagerLib () {
    EventEmitter.call(this)

    var self = this

    this.found = function (address, name) {
      self.emit('found', address, name)
    }

    this.finish = function () {
      self.emit('finished')
    }
    util.inherits(WFBSManagerLib, EventEmitter)
  }

  /**
     * 库初始化
     * @param serverip 服务器IP
     * @param serverport 服务器端口
     * @constructor
     */
  WFBSManagerLib.prototype.Initialize = function (serverip, serverport, errcallback) {
    var self = this
    self.managerWrap = WFBSManagerWrap
    self.managerWrap.Initialize(serverip, serverport, errcallback)
  }

  WFBSManagerLib.prototype.RegistHandlers = function (pktid, callback) {
    var self = this
    if (self.managerWrap) {
      self.managerWrap.RegistHandlers(pktid, callback)
    }
  }

  WFBSManagerLib.prototype.Login = function (username, pwd, callback) {
    var self = this
    if (!self.LoginWrap) {
      self.LoginWrap = new WFBSManagerWrap.LoginWrap()
    }
    self.LoginWrap.Login(username, pwd, callback)
  }

  // util.inherits(WFBSManagerLib, EventEmitter);
  exports.WFBSManagerLib = WFBSManagerLib
}())
