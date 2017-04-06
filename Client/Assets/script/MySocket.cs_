using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using Acoross.tw.Rpc;
using System;

public class MySocket : MonoBehaviour
{
    class RpcClient : Acoross.tw.Rpc.RpcClientSync
    {
        MySocket sock;

        public RpcClient(MySocket sock)
        {
            this.sock = sock;
        }

        public override void NotiDashEnd(vec2 message)
        {
            sock.NotiDashEnd(message);
        }

        public override void NotiPosSync(vec2 message)
        {
            sock.NotiPosSync(message);
        }
    }

    private RpcClient rpcclient;
    bool login = false;

    public string ipaddress = "127.0.0.1";
    public const int port = 7777;

    void ConnectAndLogin(RpcClient client)
    {
        rpcclient.Connect(IPAddress.Parse(ipaddress), port);
        Debug.Log("Client connected");

        var result = Login("1", "0");
        Debug.Log("login: " + result);
    }

    void ValidateClientPosition(vec2 pos)
    {
        Debug.Log("validate");
        var clientPos = transform.position;
        var position = new Vector3(pos.x, pos.y, clientPos.z);

        var sqMag = (position - clientPos).sqrMagnitude;
        if (sqMag > 0.1f)
        {
            Debug.Log("val: " + sqMag);
            transform.position = position;
        }
    }

    public void NotiPosSync(vec2 pos)
    {
        //ValidateClientPosition(pos);
    }

    public void NotiDashEnd(vec2 pos)
    {
        ValidateClientPosition(pos);

        var dashScript = GetComponent<Dash>();
        dashScript.DashEnd();
    }

    void Awake()
    {
        Debug.Log("Client Awake()");

        rpcclient = new RpcClient(this);
        ConnectAndLogin(rpcclient);
    }

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (!rpcclient.Running)
        {
            try
            {
                rpcclient.HandleNoties();
                rpcclient.Dispose();
            }
            catch (Exception ex)
            {
                Debug.Log(ex);
            }

            try
            {
                Debug.Log("update rpcconnect");
                rpcclient = new RpcClient(this);
                ConnectAndLogin(rpcclient);
            }
            catch (Exception ex)
            {
                Debug.Log(ex);
            }
        }
        else
        {
            try
            {
                rpcclient.HandleNoties();
            }
            catch (Exception ex)
            {
                Debug.Log(ex);
            }
        }
    }

    public bool Login(string id, string pwd)
    {
        if (login)
            return true;

        login = rpcclient.Login(new LoginReq
        {
            id = id,
            pwd = pwd
        });

        Debug.Log("login: " + (login ? "success" : "failed"));
        return login;
    }
    
    public bool Dash(Vector2 movDir, float speed, float actionTime, int effect_count)
    {
        Debug.Log("MySocket::Dash! 0");

        if (!login)
            return false;

        Debug.Log("MySocket::Dash! 1");

        if (rpcclient == null)
            return false;

        Debug.Log("MySocket::Dash! 2");

        var req = new DashReq();
        req.orgPos = new vec2();
        req.orgPos.x = transform.position.x;
        req.orgPos.y = transform.position.y;

        req.dir = new vec2();
        req.dir.x = movDir.x;
        req.dir.y = movDir.y;

        req.speed = speed;
        req.actionTime = actionTime;
        req.effectCount = effect_count;

        return rpcclient.Dash(req);
    }

    void OnApplicationQuit()
    {
        if (rpcclient != null)
        {
            rpcclient.Dispose();
        }
    }
}