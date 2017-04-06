using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyInputManager : MonoBehaviour {
    bool firing = false;
    bool moving = false;
    
    // Use this for initialization
    void Start () {
		
	}
	
    // Update is called once per frame
	void Update () {
        SetRotationByMousePosition();
        MoveByKey();

        Fire1();
        Fire2();
        Fire3();
        Jump();

        MoveCamera();
    }

    void SetRotationByMousePosition() {
        var pos = transform.position;
        var pos2d = new Vector2(pos.x, pos.y);

        var mouseWorldPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        var mouse2d = new Vector2(mouseWorldPos.x, mouseWorldPos.y);
        var relativeMouse2d = mouse2d - pos2d;

        var radAng = Mathf.Atan2(relativeMouse2d.y, relativeMouse2d.x);
        var ang = radAng * 180 / Mathf.PI + Mathf.PI / 2;

        transform.rotation = Quaternion.AngleAxis(ang, Vector3.forward);
    }

    void MoveByKey() {
        if (!moving)
        {
            moving = true;

            float forwardSpeed = 5f;
            var vert = Input.GetAxis("Vertical");
            transform.Translate(Vector2.right * vert * forwardSpeed * Time.deltaTime, Space.Self);

            //float sideSpeed = 2f;
            //var hor = Input.GetAxis("Horizontal");
            //transform.Translate(Vector2.down * hor * sideSpeed * Time.deltaTime, Space.Self);

            moving = false;
        }
    }

    void MoveCamera() {
        Camera.main.transform.position = new Vector3(
            transform.position.x, 
            transform.position.y, 
            Camera.main.transform.position.z);
    }
    
    void Fire1() {
        if (!firing) {
            bool fire1 = Input.GetButton("Fire1");
            if (fire1) {
                var shuriken = GetComponent<Shuriken1>();
                shuriken.Run();
            }   
        }
    }
    
    void Fire2() {
        if (!firing) {
            bool fire2 = Input.GetButton("Fire2");
            if (fire2) {
                var shuriken = GetComponent<Shuriken2>();
                shuriken.Run();
            }
        }
    }
    
    void Fire3()
    {
        if (!firing)
        {
            bool fire3 = Input.GetButton("Fire3");
            if (fire3)
            {
                var sword = GetComponent<Sword>();
                sword.Run();
            }
        }
    }

    void Jump() {
        if (!moving) {
            bool dash = Input.GetButton("Jump");
            if (dash) {
                var dashScript = GetComponent<Dash>();
                dashScript.Run();
            }
        }
    }
    
    void OnCollisionEnter2D(Collision2D collision) {
        Debug.Log("fire collided");
        Debug.Log(collision.gameObject.name);
    }
}
