using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveFire : MonoBehaviour {
    float timeAccum = 0f;
    public float speed = 10f;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        timeAccum += Time.deltaTime;
        transform.Translate(Vector2.right * speed * Time.deltaTime, Space.Self);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.name != "player")
        {
            Destroy(collision.gameObject);
            Destroy(gameObject);
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        Debug.Log("fire collided");
        Debug.Log(collision.gameObject.name);
    }
}
