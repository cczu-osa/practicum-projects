<template>
  <div id="user">
    <h2>用户管理</h2>
    <div id="userMain">
      <div v-if="showing === 'list'">
        <h5>对用户进行管理操作。</h5>
        <div class="row">
          <!-- table -->
          <table class="highlight">
            <thead>
              <tr>
                <th>用户 ID</th>
                <th>登录名</th>
                <th>登录密码</th>
                <th>用户角色</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(user, index) in userArray" v-on:click="userEdit(index)">
                <td>{{ user.id }}</td>
                <td>{{ user.username }}</td>
                <td>{{ user.password }}</td>
                <td>{{ user.role }}</td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="row">
          <!-- button -->
          <a class="waves-effect waves-light btn" v-on:click="userAdd">添加</a>
        </div>
      </div>

      <div class="row" v-if="showing === 'edit'">
        <h5>编辑/创建用户</h5>
        <!-- form -->
        <div class="row frm">
          <div class="input-field col s6">
            <input id="user_id" type="text" class="validate" v-model="editing.id" disabled>
            <label for="user_id">用户 ID号</label>
          </div>
        </div>

        <div class="row frm">
          <div class="input-field col s6">
            <input id="username" type="text" class="validate" v-model="editing.username">
            <label for="username">用户名</label>
          </div>
        </div>

        <div class="row frm">
          <div class="input-field col s6">
            <input id="password" type="text" class="validate" v-model="editing.password">
            <label for="password">登录密码</label>
          </div>
        </div>

        <div class="row frm">
          <div class="input-field col s6">
            <select>
              <option value="" disabled selected>Choose your option</option>
              <option v-for="(role, index) in roleList" :value="index"> {{ role }} </option>
            </select>
            <label>用户角色</label>
          </div>
        </div>

        <div class="row frm">
          <!-- button -->
          <div class="input-field inline col s6">
            <a class="waves-effect waves-light btn" v-on:click="back">返回</a>
            <a class="waves-effect waves-light btn" v-on:click="userSave">保存</a>
            <a class="waves-effect waves-light btn red right" v-on:click="userDelete" v-if="this.editing.id !== ''">删除</a>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import common from './common.js';

export default {
  name: 'user',
  data () {
    return {
      userArray: [],
      showing: 'list',
      roleList: [],
      editing: { id: '', username: '', password: '', role: ''}
    }
  },
  methods: {
    userAdd: function () {
      this.switchto('edit', () => {
        document.querySelector('select').value = common.CATEGORY[this.editing.category];
      });
    },
    userEdit: function (index) {
      this.editing = JSON.parse(JSON.stringify(this.userArray[index]));
      
      this.switchto('edit', () => {
        document.querySelector('select').value = common.ROLE[this.editing.role];
      });
    },
    userSave: function () {
      this.editing.id = encodeURIComponent(this.editing.id);
      this.editing.role = encodeURIComponent(common.roleToString(document.querySelector('select').value));
      this.editing.username = encodeURIComponent(this.editing.username);
      this.editing.password = encodeURIComponent(this.editing.password);

      let finishHandler = (data) => {
        if (data.body.status === 'ok') {
          this.refresh();
          this.back();
        }
        else {
          Materialize.toast('权限不足，只有管理员可以修改和创建用户', 4000)
        }
      }

      if (this.editing.id !== '') {
        this.$http.post(`/api/v1/user?token=${this.$parent.accessToken}&id=${this.editing.id}&role=${this.editing.role}&username=${this.editing.username}&password=${this.editing.password}`).then(finishHandler)
      }
      else {
        this.$http.put(`/api/v1/user?token=${this.$parent.accessToken}&role=${this.editing.role}&username=${this.editing.username}&password=${this.editing.password}`).then(finishHandler)
      }
    },
    userDelete: function () {
      this.editing.id = encodeURIComponent(this.editing.id);
      if (this.editing.id !== '') {
        this.$http.delete(`/api/v1/user?token=${this.$parent.accessToken}&id=${this.editing.id}`).then((data) => {
          if (data.body.status === 'ok') {
            this.refresh();
            this.back();
          }
          else {
            Materialize.toast('权限不足，只有管理员可以修改和创建用户', 4000)
          }
        })
      }
    },
    back: function () {
      this.editing = { id: '', username: '', password: '', role: ''};
      this.switchto('list');
    },
    switchto: function (dest, callback) {
      let content = document.querySelector('#userMain');
      content.style.opacity = 0;
      setTimeout(() => {
        this.showing = dest;
        if (callback) setTimeout(() => {callback();}, 0);
        content.style.opacity = 1;
      }, 100);
    },
    refresh: function() {
      this.$http.get(`/api/v1/user?token=${this.$parent.accessToken}`).then((response) => {
        this.userArray = response.body.dataset;
      })
    }
  },
  created: function () {
    let accessToken = this.$parent.accessToken;
    if (!accessToken || accessToken === '')
      this.$parent.tabNavigate(-1);

    for (let key in common.ROLE) {
      this.roleList.push(key);
    }

    this.refresh();
  },
  watch: {
    showing: function(val) {
      setTimeout(() => {
        $('select').material_select();
        Materialize.updateTextFields(); 
      }, 0);
    }
  }
}
</script>

<style scoped>
  #user {
    max-width: 800px;
    margin: 0 auto;
  }
  #userMain {
    transition: all ease 0.1s;
  }
  h2 {
    font-size: 22px;
  }
  h5 {
    font-size: 16px;
  }
  div.frm {
    margin-bottom: 0;
  }
</style>
