<template>
  <div id="officer">
    <h2>执法人员管理</h2>
    <div id="officerMain">

      <div v-if="showing === 'list'">
        <h5>对执法人员进行管理操作。</h5>
        <div class="row">
          <table class="highlight">
            <thead>
              <tr>
                <th>执法人员编号</th>
                <th>执法人员名字</th>
                <th>执法人员性别</th>
                <th>执法人员专业</th>
                <th>执法人员职务</th>
                <th>执法人员许可编号</th>
                <th>执法人员工作地区</th>
                <th>执法人员用户名</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(officer, index) in officerArray" v-on:click="officerEdit(index)">
                <td> {{ officer.id }} </td>
                <td> {{ officer.name }} </td>
                <td> {{ officer.gender }} </td>
                <td> {{ officer.major }} </td>
                <td> {{ officer.job }} </td>
                <td> {{ officer.license }} </td>
                <td> {{ localeArray.filter(item => item.id === officer.locale_id)[0].value }} </td>
                <td> {{ userArray.filter(item => item.id === officer.uid)[0].username }} </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="row">
          <a class="waves-effect waves-light btn" v-on:click="officerAdd">新增</a>
        </div>
      </div>

      <div v-if="showing === 'edit'">
        <h5>创建/编辑执法人员。</h5>

        <div class="row frm">
          <div class="input-field col s4">
            <input id="id" type="text" class="validate" v-model="editing.id" disabled>
            <label for="id">执法人员 ID</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <input id="name" type="text" class="validate" v-model="editing.name">
            <label for="name">执法人员名字</label>
          </div>
          <div class="input-field col s4">
            <select id="genderSelector">
              <option value="" disabled selected>选择性别</option>
              <option value="男">男</option>
              <option value="女">女</option>
            </select>
            <label>执法人员性别</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <input id="major" type="text" class="validate" v-model="editing.major">
            <label for="major">执法人员专业</label>
          </div>
          <div class="input-field col s4">
            <input id="job" type="text" class="validate" v-model="editing.job">
            <label for="job">执法人员职务</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <input id="license" type="text" class="validate" v-model="editing.license">
            <label for="license">执法人员许可编号</label>
          </div>
          <div class="input-field col s4">
            <select id="localeSelector">
              <option value="" disabled selected>选择地区</option>
              <option v-for="(locale, index) in localeArray" :value="locale.id"> {{ locale.value }} </option>
            </select>
            <label>执法人员工作地区</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <select id="userSelector">
              <option value="" disabled selected>选择用户</option>
              <option v-for="(user, index) in userArray" :value="user.id"> {{ user.username }} </option>
            </select>
            <label>执法人员用户 ID</label>
          </div>
        </div>
        <div class="row frm">
          <!-- button -->
          <div class="input-field inline col s8">
            <a class="waves-effect waves-light btn" v-on:click="back">返回</a>
            <a class="waves-effect waves-light btn" v-on:click="officerSave">保存</a>
            <a class="waves-effect waves-light btn red right" v-on:click="officerDelete" v-if="this.editing.id !== ''">删除</a>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'officer',
  data () {
    return {
      officerArray: [],
      localeArray: [],
      userArray: [],
      showing: 'list',
      editing: {},
    }
  },
  methods: {
    refresh: function() {
      this.$http.get(`/api/v1/officer?token=${this.$parent.accessToken}`).then((response) => {
        this.officerArray = response.body.dataset;
      })
    },
    officerAdd: function() {
      this.editing.id = '';
      this.editing.name = '';
      this.editing.gender = '';
      this.editing.major = '';
      this.editing.job = '';
      this.editing.license = '';
      this.editing.locale_id = '';
      this.editing.uid = '';
      this.switchto('edit');
    },
    officerEdit: function(index) {
      this.editing = JSON.parse(JSON.stringify(this.officerArray[index]));
      this.switchto('edit', () => {
        document.querySelector('#genderSelector').value = this.editing.gender;
        document.querySelector('#localeSelector').value = this.editing.locale_id;
        document.querySelector('#userSelector').value = this.editing.uid;
      });
    },
    officerSave: function() {
      this.editing.gender = document.querySelector('#genderSelector').value;
      this.editing.locale_id = document.querySelector('#localeSelector').value;
      this.editing.uid = document.querySelector('#userSelector').value;
      let query = [];
      for (let _ in this.editing) {
        if (_ === 'id' && this.editing.id === '')
          continue;
        query.push(`${_}=${encodeURIComponent(this.editing[_])}`);
      }
      query = query.join('&');

      let finishHandler = (data) => {
        if (data.body.status === 'ok') {
          this.refresh();
          this.back();
        }
        else {
          Materialize.toast('权限不足，只有管理员可以修改和创建执法人员信息', 4000)
        }
      }

      if (this.editing.id === '') {
        this.$http.put(`/api/v1/officer?token=${this.$parent.accessToken}&${query}`).then(finishHandler)
      }
      else {
        this.$http.post(`/api/v1/officer?token=${this.$parent.accessToken}&${query}`).then(finishHandler)
      }
    },
    officerDelete: function() {
      if (this.editing.id !== '') {
        this.$http.delete(`/api/v1/officer?token=${this.$parent.accessToken}&id=${this.editing.id}`).then((data) => {
          if (data.body.status === 'ok') {
            this.refresh();
            this.back();
          }
          else {
            Materialize.toast('权限不足，只有管理员可以删除执法人员信息', 4000)
          }
        })
      }
    },
    switchto: function (dest, callback) {
      let content = document.querySelector('#officerMain');
      content.style.opacity = 0;
      setTimeout(() => {
        this.showing = dest;
        if (callback) setTimeout(() => {callback();}, 0);
        content.style.opacity = 1;
      }, 100);
    },
    back: function() {
      this.switchto('list', () => { this.refresh(); });
    }
  },
  created: function () {
    let accessToken = this.$parent.accessToken;
    if (!accessToken || accessToken === '')
      this.$parent.tabNavigate(-1);

    // Get all locale and user
    this.$http.get(`/api/v1/dict?token=${this.$parent.accessToken}&category=LOCALE`).then((response) => {
      this.localeArray = response.body.dataset;
    })
    this.$http.get(`/api/v1/user?token=${this.$parent.accessToken}&role=OFFICER`).then((response) => {
      this.userArray = response.body.dataset;
    })

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
  #officer {
    max-width: 1000px;
    margin: 0 auto;
  }
  #officerMain {
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